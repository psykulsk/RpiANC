//
// Created by pitersk on 20.03.19.
//

#include <omp.h>
#include "../Headers/delay_tests.h"

long single_delay_check(snd_pcm_uframes_t frames_in_play_period, snd_pcm_uframes_t frames_in_cap_period,
                        snd_pcm_t *play_handle, snd_pcm_t *cap_handle, std::ifstream &noise_file,
                        std::ofstream &output_file, bool generate_audio) {


    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool start = true;
    bool peak_found = false;

    ssize_t bytes_in_play_period = snd_pcm_frames_to_bytes(play_handle, frames_in_play_period);
    ssize_t bytes_in_cap_period = snd_pcm_frames_to_bytes(cap_handle, frames_in_cap_period);

    fixed_sample_type capture_buffer[frames_in_cap_period * NR_OF_CHANNELS];
    fixed_sample_type play_buffer[frames_in_play_period * NR_OF_CHANNELS];
    long delay_us;
    long nr_of_samples = 100;


    auto silence_samples = GeneratedAudio<PLAY_FRAMES_PER_PERIOD * NR_OF_CHANNELS>(true);
    auto pitch_samples = GeneratedAudio<PLAY_FRAMES_PER_PERIOD * NR_OF_CHANNELS>(false);

    long nr_of_silence_samples = random() % 16;
    bool info_printed = false;

    omp_lock_t capture_lock;
    omp_init_lock(&capture_lock);
    omp_set_lock(&capture_lock);
#pragma omp parallel sections
    {
#pragma omp section
        {
//            unsigned int rand_sleep_time = 100 + std::rand() % 500;
//            usleep(rand_sleep_time);
            int sample = 0;
            while (sample < nr_of_samples) {
                ++sample;
                if (!generate_audio) {
                    noise_file.read((char *) play_buffer, bytes_in_play_period);
                    if (start) {
                        start_time = std::chrono::high_resolution_clock::now();
                        start = false;
//                        std::cout << "Freeing lock" << std::endl;
                        omp_unset_lock(&capture_lock);
                    }
                    playback(play_handle, play_buffer, frames_in_play_period);
                } else {
                    fixed_sample_type *play_samples;
                    if (sample < nr_of_silence_samples)
                        play_samples = silence_samples.sample_array;
                    else {
                        if (start) {
                            start_time = std::chrono::high_resolution_clock::now();
                            start = false;
//                            std::cout << "Freeing lock" << std::endl;
                            omp_unset_lock(&capture_lock);
                        }
                        play_samples = pitch_samples.sample_array;
                    }
                    playback(play_handle, play_samples, frames_in_play_period);
                }
            }
        }
#pragma omp section
        {
            int sample = 0;
            omp_set_lock(&capture_lock);
            while (sample < nr_of_samples) {
                ++sample;
                capture(cap_handle, capture_buffer, frames_in_cap_period);
//                double sum = 0.0;
                for (unsigned long i = 0; i < frames_in_cap_period * NR_OF_CHANNELS && !peak_found; ++i) {
                    if (i % 2 && i > 1) {
//                        float sample_abs_val =
//                                (float) capture_buffer[i] / (float) std::numeric_limits<fixed_sample_type>::max();
                        float diff = ((float) capture_buffer[i] - (float) capture_buffer[i - 2]) /
                                     (float) std::numeric_limits<fixed_sample_type>::max();
                        if (diff > 0.001 && diff < 0.01) {
                            end_time = std::chrono::high_resolution_clock::now();
                            peak_found = true;
                            std::cout << "Peak found after " << sample << " periods. Peak val: " <<
                                      diff
                                      << std::endl;
                            break;
                        }
//                        sum += (double) (std::abs(capture_buffer[i]));
                    }
                }
                output_file.write((char *) capture_buffer, bytes_in_cap_period);
//                double avg = sum / ((double) std::numeric_limits<fixed_sample_type>::max() *
//                                    (double) frames_in_cap_period);
//                std::cout << "Avg: " << avg << std::endl;
                if (peak_found && !info_printed) {
                    delay_us = std::chrono::duration_cast<std::chrono::microseconds>(
                            end_time - start_time).count();
                    std::cout << "Delay: " << delay_us << " us" << std::endl;
                    info_printed = true;
                }
            }
        }
    }
    omp_destroy_lock(&capture_lock);


    if (peak_found) {
        return delay_us;
    } else {
        std::cout << "Peak not found" << std::endl;
        return -1;
    }

}

double std_deviation(std::vector<long> delays, double mean) {
    double var = 0;
    for (long delay : delays) {
        double delayf = (double) delay;
        var += (delayf - mean) * (delayf - mean);
    }
    var /= (double) delays.size();
    return std::sqrt(var);
}

