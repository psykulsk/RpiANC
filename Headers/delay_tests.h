//
// Created by pitersk on 20.03.19.
//

#ifndef RPIANC_DELAY_TESTS_H
#define RPIANC_DELAY_TESTS_H

#include <alsa/asoundlib.h>
#include <iostream>
#include <fstream>
#include <numeric>
#include <chrono>
#include <cmath>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/common.h"
#include "../Headers/constants.h"

template<int nr_of_samples>
struct GeneratedAudio {
    explicit GeneratedAudio(bool silent) : sample_array() {
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

long single_delay_check(snd_pcm_uframes_t frames_in_play_period, snd_pcm_uframes_t frames_in_cap_period,
                        snd_pcm_t *play_handle, snd_pcm_t *cap_handle, std::ifstream &noise_file,
                        bool generate_audio = false);


double std_deviation(std::vector<long> delays, double mean);

#endif //RPIANC_DELAY_TESTS_H
