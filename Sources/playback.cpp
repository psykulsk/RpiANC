//
// Created by pitersk on 27.05.18.
//

#include "../Headers/playback.h"
#include <iostream>


void init_playback(snd_pcm_t **handle, unsigned int *play_freq, snd_pcm_uframes_t *play_period_size,
                   snd_pcm_uframes_t *play_buffer_size,
                   unsigned int number_of_channels, const std::string playback_device_name) {

    snd_pcm_hw_params_t *params;
    int rc;
    int dir;
    int val;

    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(handle, playback_device_name.c_str(),
                      SND_PCM_STREAM_PLAYBACK, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }


    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_malloc(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(*handle, params);


    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(*handle, params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(*handle, params,
                                 SND_PCM_FORMAT_S32_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(*handle, params, number_of_channels);

    /* 44100 bits/second sampling rate (CD quality) */

    snd_pcm_hw_params_set_rate_near(*handle, params,
                                    play_freq, &dir);

    snd_pcm_hw_params_set_buffer_size_near(*handle, params, play_buffer_size);
    snd_pcm_hw_params_set_period_size_near(*handle, params, play_period_size, &dir);
    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(*handle, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_get_rate(params, play_freq, &dir);
    snd_pcm_hw_params_get_period_size(params, play_period_size, &dir);
    snd_pcm_hw_params_get_buffer_size(params, play_buffer_size);

    std::cerr << "Playback params: " << "Capture rate: " << *play_freq << " Period size: "
              << *play_period_size <<
              " Buffer size: " << *play_buffer_size << std::endl;

    snd_pcm_hw_params_free(params);
}

void playback(snd_pcm_t *play_handle, fixed_sample_type *play_buffer,
              snd_pcm_uframes_t play_period_size) {
    ssize_t rc;
    rc = snd_pcm_writei(play_handle, play_buffer, play_period_size);
    if (rc == -EPIPE) {
        /* EPIPE means underrun */
        fprintf(stderr, "playback underrun occurred\n");
        snd_pcm_prepare(play_handle);
    } else if (rc == -EAGAIN) {
        //fprintf(stderr, "EAGAIN writei not available\n");
    } else if (rc < 0) {
        fprintf(stderr,
                "playback error from writei: %s\n",
                snd_strerror(rc));
    } else if (rc != (int) play_period_size) {
        fprintf(stderr,
                "playback short write, write %d frames\n", rc);
    }
}