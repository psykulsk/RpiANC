//

#include <iostream>
#include <fstream>
#include <algorithm>
#include "../Headers/delay_tests.h"

#define DEPLOYED_ON_RPI

const bool SILENT = false;

const size_t PLAY_FRAMES_PER_PERIOD = 512;
const size_t PLAY_PERIODS_PER_BUFFER = 8;
const size_t CAP_FRAMES_PER_PERIOD = 64;
const size_t CAP_PERIODS_PER_BUFFER = 8;

template<int nr_of_samples>
struct GeneratedAudio {
    GeneratedAudio(bool silent) : sample_array() {
        for (auto i = 0; i < nr_of_samples; ++i) {
            if (silent) {
                sample_array[i] = 0;
            } else {
                if (i % 2)
                    sample_array[i] = std::numeric_limits<fixed_sample_type>::max();
                else
                    sample_array[i] = std::numeric_limits<fixed_sample_type>::min();
            }
        }
    }

    fixed_sample_type sample_array[nr_of_samples];
};

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
    snd_pcm_uframes_t cap_frames_per_period = CAP_FRAMES_PER_PERIOD;

    init_capture(&cap_handle, &play_freq, &cap_frames_per_period, number_of_channels,
                 capture_device_name);

    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_frames_per_period = PLAY_FRAMES_PER_PERIOD;
    snd_pcm_uframes_t play_frames_per_device_buffer = PLAY_PERIODS_PER_BUFFER * PLAY_FRAMES_PER_PERIOD;

    init_playback(&play_handle, &play_freq, &play_frames_per_period,
                  &play_frames_per_device_buffer, number_of_channels, playback_device_name);

    fixed_sample_type capture_buffer[cap_frames_per_period * NR_OF_CHANNELS];
    auto generated_audio = GeneratedAudio<PLAY_FRAMES_PER_PERIOD * NR_OF_CHANNELS>(SILENT);
    std::vector<long> delay_test_results;
    std::ifstream noise_file;

    for (unsigned long i = 0; i < number_of_tests; ++i) {
        long delay = single_delay_check(play_frames_per_period, cap_frames_per_period, play_handle,
                                        cap_handle, noise_file, generated_audio.sample_array);
        if (delay != -1) {
            delay_test_results.push_back(delay);
        }
        usleep(50000);
        for (int j = 0; j < 50; j++) {
            capture(cap_handle, capture_buffer, cap_frames_per_period);
        }
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

        std::cout << "Min: " << min << " Max: " << max << " avg: " << average << " median: "
                  << delay_test_results.at(delay_test_results.size() / 2)
                  << " std deviation: " << deviation << std::endl;
    }

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}
