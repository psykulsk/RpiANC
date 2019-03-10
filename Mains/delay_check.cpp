//

#include <iostream>
#include <fstream>
#include <numeric>
#include <chrono>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"
#include <algorithm>
#include <omp.h>

#define DEPLOYED_ON_RPI

const size_t NR_OF_CHANNELS = 2;

long
single_delay_check(snd_pcm_uframes_t frames_in_play_period, snd_pcm_uframes_t frames_in_cap_period,
                   snd_pcm_t *play_handle, snd_pcm_t *cap_handle,
                   std::ifstream &noise_file) {


    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool start = true;
    bool peak_found = false;

    noise_file.clear();
    noise_file.seekg(0, std::ios::beg);

    ssize_t bytes_in_play_period = snd_pcm_frames_to_bytes(play_handle, frames_in_play_period);

    fixed_sample_type play_buffer[frames_in_play_period * NR_OF_CHANNELS];
    fixed_sample_type capture_buffer[frames_in_cap_period * NR_OF_CHANNELS];
    long delay_us;
    long nr_of_samples = 1000;

    fixed_sample_type threshold = std::numeric_limits<fixed_sample_type>::max() * 0.052f;

#pragma omp parallel sections
    {
#pragma omp section
        {
            int sample = 0;
            while (sample < nr_of_samples) {
                if (start) {
                    start_time = std::chrono::high_resolution_clock::now();
                    start = false;
                }
                ++sample;
                noise_file.read((char *) play_buffer, bytes_in_play_period);
                playback(play_handle, play_buffer, frames_in_play_period);
            }
        }
#pragma omp section
        {
            bool info_printed = false;
            int sample = 0;
            while (sample < nr_of_samples * 4) {
                ++sample;
                capture(cap_handle, capture_buffer, frames_in_cap_period);
//                double sum = 0.0;
                for (unsigned long i = 0;
                     i < frames_in_cap_period * NR_OF_CHANNELS && !peak_found; ++i) {
                    if (i % 2) {
                        if (std::abs(capture_buffer[i]) > threshold) {
                            end_time = std::chrono::high_resolution_clock::now();
                            peak_found = true;
                            std::cout << "Peak found" << std::endl;
                            break;
                        }
//                        sum += (double) (std::abs(capture_buffer[i]));
                    }
                }
//                double avg = sum / (std::numeric_limits<fixed_sample_type>::max() *
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


    if (peak_found) {
        return delay_us;
    } else {
        std::cout << "Peak not found" << std::endl;
        return -1;
    }

}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_TEST_REPETITIONS>" << std::endl;
        return 1;
    }

    unsigned long number_of_tests;
    try {
        number_of_tests = std::stoul(argv[1], nullptr);
    } catch (std::invalid_argument const &e) {
        std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_TEST_REPETITIONS>" << std::endl;
        return 1;
    }


    std::vector<fixed_sample_type> record_vec;


#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = "hw:CARD=sndrpisimplecar,DEV=0";
    const std::string playback_device_name = "plughw:CARD=ALSA,DEV=0";
#else
    const std::string capture_device_name = "default";
    const std::string playback_device_name = "default";
#endif

    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = 2;
    snd_pcm_uframes_t cap_frames_per_period = 64;

    init_capture(&cap_handle, &play_freq, &cap_frames_per_period, number_of_channels,
                 capture_device_name);

    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_frames_per_period= 256;
    snd_pcm_uframes_t play_frames_per_device_buffer = 8 * play_frames_per_period;

    init_playback(&play_handle, &play_freq, &play_frames_per_period,
                  &play_frames_per_device_buffer, number_of_channels, playback_device_name);

    fixed_sample_type capture_buffer[cap_frames_per_period*NR_OF_CHANNELS];
    std::vector<long> delay_test_results;
    std::ifstream noise_file("tone_sine_5k.raw", std::ios::binary | std::ios::in);
    if (!noise_file.is_open()) {
        std::cout << "File not opened" << std::endl;
        return -1;
    }
    if (!noise_file.good()) {
        std::cout << "File not good" << std::endl;
        return -1;
    }

    for (unsigned long i = 0; i < number_of_tests; ++i) {
        long delay = single_delay_check(play_frames_per_period, cap_frames_per_period, play_handle,
                                        cap_handle,
                                        noise_file);
        if (delay != -1) {
            delay_test_results.push_back(delay);
        }
        usleep(50000);
        for (int j = 0; j < 3000; j++) {
            capture(cap_handle, capture_buffer, cap_frames_per_period);
        }
    }

    noise_file.close();

    if (delay_test_results.size() > 1) {
//        Remove first, flawed element of results
        delay_test_results.erase(delay_test_results.begin());
        std::sort(delay_test_results.begin(), delay_test_results.end());
        long min = *delay_test_results.begin();
        long max = *(delay_test_results.end() - 1);

        double average = 0;
        for (long &delay : delay_test_results) {
            average += static_cast<double>(delay);
        }
        average /= static_cast<double>(delay_test_results.size());

        std::cout << "Min: " << min << " Max: " << max << " avg: " << average << " median: "
                  << delay_test_results.at(delay_test_results.size() / 2) << std::endl;
    }

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

