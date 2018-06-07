//
// Created by pitersk on 27.05.18.
//

#ifndef ONELOOPCPP_CAPTURE_H
#define ONELOOPCPP_CAPTURE_H


/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <string>

typedef long cap_sample_type;


void init_capture(snd_pcm_t **cap_handle, unsigned int *cap_freq, snd_pcm_uframes_t *cap_period_size,
                  unsigned int number_of_channels, const std::string capture_device_name);

void capture(snd_pcm_t *cap_handle, cap_sample_type *cap_buffer, snd_pcm_uframes_t cap_period_size);

#endif //ONELOOPCPP_CAPTURE_H
