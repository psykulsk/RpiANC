//

#include <iostream>
#include <fstream>
#include <numeric>
#include <chrono>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"
#include <algorithm>

#define DEPLOYED_ON_RPI

long single_delay_check(long play_buffer_length, snd_pcm_uframes_t buffer_length,
                        snd_pcm_uframes_t cap_period_size,
                        snd_pcm_t *play_handle, snd_pcm_t *cap_handle) {

    std::ifstream noise_file("short_tone.raw", std::ios::binary);
    assert(noise_file.is_open());

    int sample = 0;
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool start = true;
    bool peak_found = false;

    fixed_sample_type play_buffer[play_buffer_length];
    fixed_sample_type capture_buffer[buffer_length];
    while (sample < 1000 && !peak_found) {

        ++sample;
        size_t size = play_buffer_length * sizeof(fixed_sample_type);
        noise_file.read((char *) play_buffer, size);
        if (start) {
            start_time = std::chrono::high_resolution_clock::now();
            start = false;
        }
        playback(play_handle, play_buffer, cap_period_size);
        capture(cap_handle, capture_buffer, cap_period_size);
        for (unsigned long i = 0; i < buffer_length; ++i) {
            if (i % 2)
                if (std::abs(capture_buffer[i]) >
                    std::numeric_limits<fixed_sample_type>::max() * 0.3) {
                    end_time = std::chrono::high_resolution_clock::now();
                    peak_found = true;
                    std::cout << "Peak found" << std::endl;
                    break;
                }
        }
    }
    noise_file.close();

    if (peak_found) {
        long delay_us = std::chrono::duration_cast<std::chrono::microseconds>(
                end_time - start_time).count();
        std::cout << "Delay: " << delay_us << " us" << std::endl;
        return delay_us;
    } else {
        std::cout << "Peak not found" << std::endl;
        return -1;
    }

}

int main(int argc, char* argv[]) {

    if(argc < 2){
        std::cerr << "Usage: " << argv[0] << " <NUMBER_OF_TEST_REPETITIONS>" << std::endl;
        return 1;
    }

    unsigned long number_of_tests;
    try {
       number_of_tests = std::stoul(argv[1], nullptr);
    } catch(std::invalid_argument const &e) {
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
    snd_pcm_uframes_t cap_period_size = 64;

    init_capture(&cap_handle, &play_freq, &cap_period_size, number_of_channels,
                 capture_device_name);
    snd_pcm_uframes_t buffer_length = cap_period_size * number_of_channels;


    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_device_buffer = 1024;
    snd_pcm_uframes_t play_period_size = 256;

    init_playback(&play_handle, &play_freq, &play_period_size,
                  &play_device_buffer, number_of_channels, playback_device_name);
    snd_pcm_uframes_t play_buffer_length = play_period_size * number_of_channels;

    std::vector<long> delay_test_results;

    for (unsigned long i = 0; i < number_of_tests; ++i) {
        long delay = single_delay_check(play_buffer_length, buffer_length, cap_period_size,
                                        play_handle, cap_handle);
        if (delay != -1) {
            delay_test_results.push_back(delay);
        }
        usleep(50000);
    }


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

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

