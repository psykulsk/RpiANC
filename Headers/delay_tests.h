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


long single_delay_check(snd_pcm_uframes_t frames_in_play_period, snd_pcm_uframes_t frames_in_cap_period,
                        snd_pcm_t *play_handle, snd_pcm_t *cap_handle, std::ifstream &noise_file,
                        fixed_sample_type *play_samples= nullptr);


double std_deviation(std::vector<long> delays, double mean);

#endif //RPIANC_DELAY_TESTS_H
