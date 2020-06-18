//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <omp.h>
#include "../Headers/delay_tests.h"
#include "../Headers/constants.h"

#define DEPLOYED_ON_RPI

const bool GENERATE_AUDIO = true;

int main(int argc, char *argv[]) {

    omp_set_num_threads(2);

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


    std::vector<long> delay_test_results;

    for (unsigned long i = 0; i < number_of_tests; ++i) {
        snd_pcm_prepare(cap_handle);
        snd_pcm_prepare(play_handle);
        long delay = single_delay_check(play_frames_per_period, cap_frames_per_period, play_handle,
                                        cap_handle, GENERATE_AUDIO);
        if (delay != -1) {
            delay_test_results.push_back(delay);
        }
        snd_pcm_drain(cap_handle);
        snd_pcm_drop(cap_handle);
        snd_pcm_drain(play_handle);
        usleep(100000);
        snd_pcm_drop(play_handle);
    }


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

        double deviation = std_deviation(delay_test_results, average);

        std::cout << CAP_FRAMES_PER_PERIOD <<" "<< min << " " << max << " " << average << " "
                  << delay_test_results.at(delay_test_results.size() / 2)
                  << " " << deviation << std::endl;
    }

    snd_pcm_close(play_handle);
    snd_pcm_close(cap_handle);

    return 0;
}
