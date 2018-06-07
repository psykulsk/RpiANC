//
// Created by pitersk on 27.05.18.
//

#ifndef ONELOOPCPP_PLAYBACK_H
#define ONELOOPCPP_PLAYBACK_H

/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <alsa/asoundlib.h>
#include <string>

typedef long play_sample_type;

void init_playback(snd_pcm_t **handle, unsigned int *play_freq, snd_pcm_uframes_t *play_period_size,
                   snd_pcm_uframes_t *play_buffer_size, unsigned int number_of_channels, const std::string playback_device_name);
void playback(snd_pcm_t *play_handle, play_sample_type *play_buffer, snd_pcm_uframes_t play_period_size);


#endif //ONELOOPCPP_PLAYBACK_H
