
#include <vector>
#include <chrono>
#include <iostream>
#include <numeric>
#include <omp.h>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"
#include "../Headers/common.h"

#define DEPLOYED_ON_RPI

typedef std::chrono::high_resolution_clock::time_point time_point;

void avg_from_vector(const std::vector<long> &vec) {
    long sum = std::accumulate(vec.begin(), vec.end(), 0);
    double avg = (double) sum / vec.size();
    std::cout << "Average: " << avg << "\n";
}

int main() {

    omp_set_num_threads(3);

#ifdef CAP_MEASUREMENTS
    const long RESERVED_SAMPLES = 25600000;
    std::vector<fixed_sample_type> err_vec;
    err_vec.reserve(RESERVED_SAMPLES);
    std::vector<fixed_sample_type> ref_vec;
    ref_vec.reserve(RESERVED_SAMPLES);
    std::vector<fixed_sample_type> corr_vec;
    corr_vec.reserve(RESERVED_SAMPLES);
    std::vector<fixed_sample_type> corr_vec_2;
    corr_vec_2.reserve(RESERVED_SAMPLES);
    std::vector<long> cap_time;
    cap_time.reserve(RESERVED_SAMPLES);
    std::vector<long> proc_time;
    proc_time.reserve(RESERVED_SAMPLES);
    std::vector<long> play_time;
    play_time.reserve(RESERVED_SAMPLES);
    std::vector<long> total_time;
    total_time.reserve(RESERVED_SAMPLES);
#endif

#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = RPI_CAPTURE_DEVICE_NAME;
    const std::string playback_device_name = RPI_PLAYBACK_DEVICE_NAME;
#else
    const std::string capture_device_name = "default";
    const std::string playback_device_name = "default";
#endif

    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = NR_OF_CHANNELS;
    snd_pcm_uframes_t cap_frames_per_period = CAP_FRAMES_PER_PERIOD;
    snd_pcm_uframes_t cap_frames_per_device_buffer = CAP_PERIODS_PER_BUFFER * CAP_FRAMES_PER_PERIOD;

    init_capture(&cap_handle, &play_freq, &cap_frames_per_period, &cap_frames_per_device_buffer, NR_OF_CHANNELS,
                 capture_device_name);

    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_frames_per_period = PLAY_FRAMES_PER_PERIOD;
    snd_pcm_uframes_t play_frames_per_device_buffer = PLAY_PERIODS_PER_BUFFER * PLAY_FRAMES_PER_PERIOD;

    init_playback(&play_handle, &play_freq, &play_frames_per_period,
                  &play_frames_per_device_buffer, number_of_channels, playback_device_name);

    int sample = 0;
    std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> capture_buffer = {0};
    std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> playback_buffer = {0};
    std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> processing_buffer = {0};
    const int START_PROCESSING_AFTER_SAMPLE = 4000;

    while (sample < 100000) {
        ++sample;
#ifdef CAP_MEASUREMENTS
        time_point total_start = std::chrono::high_resolution_clock::now();
#endif
#pragma omp parallel sections
        {
#pragma omp section
            {
#ifdef CAP_MEASUREMENTS
                time_point start = std::chrono::high_resolution_clock::now();
#endif
                capture(cap_handle, capture_buffer.data(), CAP_FRAMES_PER_PERIOD);
                dc_removal(capture_buffer.data(), BUFFER_SAMPLE_SIZE);
#ifdef CAP_MEASUREMENTS
                time_point end = std::chrono::high_resolution_clock::now();
                cap_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
                for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i)
                    if (i % 2)
                        err_vec.push_back(capture_buffer[i]);
                    else
                        ref_vec.push_back(capture_buffer[i]);
#endif
            }
#pragma omp section
            {
#ifdef CAP_MEASUREMENTS
                time_point start = std::chrono::high_resolution_clock::now();
#endif
                if (sample > START_PROCESSING_AFTER_SAMPLE) {
                    processing_feedforward_anc(processing_buffer.data(), BUFFER_SAMPLE_SIZE);
                }
#ifdef CAP_MEASUREMENTS
                time_point end = std::chrono::high_resolution_clock::now();
                proc_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
                for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i) {
                    if (i % 2)
                        corr_vec_2.push_back(processing_buffer[i]);
                    else
                        corr_vec.push_back(processing_buffer[i]);
                }
#endif

            }
#pragma omp section
            {
#ifdef CAP_MEASUREMENTS
                time_point start = std::chrono::high_resolution_clock::now();
#endif
                playback(play_handle, playback_buffer.data(), PLAY_FRAMES_PER_PERIOD);
#ifdef CAP_MEASUREMENTS
                time_point end = std::chrono::high_resolution_clock::now();
                play_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
#endif
            }
        }
// Exchange data between arrays
        std::copy(processing_buffer.begin(), processing_buffer.end(), playback_buffer.begin());
        std::copy(capture_buffer.begin(), capture_buffer.end(), processing_buffer.begin());
#ifdef CAP_MEASUREMENTS
        time_point total_end = std::chrono::high_resolution_clock::now();
        total_time.push_back(std::chrono::duration_cast<std::chrono::microseconds>(total_end - total_start).count());
#endif
    }


#ifdef CAP_MEASUREMENTS
    std::cout << "Cap time" << "\n";
    avg_from_vector(cap_time);
    std::cout << "proc time" << "\n";
    avg_from_vector(proc_time);
    std::cout << "play time" << "\n";
    avg_from_vector(play_time);
    std::cout << "total time" << "\n";
    avg_from_vector(total_time);

    save_vector_to_file("rec/cap_time.dat", cap_time);
    save_vector_to_file("rec/proc_time.dat", proc_time);
    save_vector_to_file("rec/play_time.dat", play_time);
    save_vector_to_file("rec/total_time.dat", total_time);
    save_vector_to_file("rec/err_mic.dat", err_vec);
    save_vector_to_file("rec/ref_mic.dat", ref_vec);
    save_vector_to_file("rec/corr_sig.dat", corr_vec);
#endif
    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

