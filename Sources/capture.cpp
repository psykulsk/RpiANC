//
// Created by pitersk on 27.05.18.
//

#include "../Headers/capture.h"
#include <iostream>

void init_capture(snd_pcm_t **cap_handle, unsigned int *cap_freq, snd_pcm_uframes_t *cap_period_size,
                  unsigned int number_of_channels, const std::string capture_device_name){

    snd_pcm_hw_params_t *params;
    int rc;
    int dir;
    int val;

    /* Open PCM device for recording (capture). */
    rc = snd_pcm_open(cap_handle, capture_device_name.c_str(),
                      SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr,
                "unable to open pcm device: %s\n",
                snd_strerror(rc));
        exit(1);
    }


    /* Allocate a hardware parameters object. */
    snd_pcm_hw_params_malloc(&params);

    /* Fill it in with default values. */
    snd_pcm_hw_params_any(*cap_handle, params);


    /* Set the desired hardware parameters. */

    /* Interleaved mode */
    snd_pcm_hw_params_set_access(*cap_handle, params,
                                 SND_PCM_ACCESS_RW_INTERLEAVED);

    /* Signed 16-bit little-endian format */
    snd_pcm_hw_params_set_format(*cap_handle, params,
                                 SND_PCM_FORMAT_S16_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(*cap_handle, params, number_of_channels);

    /* 44100 bits/second sampling rate (CD quality) */

    snd_pcm_hw_params_set_rate_near(*cap_handle, params,
                                    cap_freq, &dir);

//    snd_pcm_hw_params_set_buffer_size_near (cap_handle, params, &frames);
    snd_pcm_hw_params_set_period_size_near (*cap_handle, params, cap_period_size, NULL);
    /* Write the parameters to the driver */
    rc = snd_pcm_hw_params(*cap_handle, params);
    if (rc < 0) {
        fprintf(stderr,
                "unable to set hw parameters: %s\n",
                snd_strerror(rc));
        exit(1);
    }

    snd_pcm_hw_params_get_rate(params, cap_freq, &dir);
    snd_pcm_hw_params_get_period_size(params, cap_period_size, &dir);

    std::cerr << "Capture params: " << "Capture rate: " << *cap_freq << " Period size: " << *cap_period_size << std::endl;

    snd_pcm_hw_params_free(params);
}

void capture(snd_pcm_t *cap_handle, sample_type *cap_buffer, snd_pcm_uframes_t cap_period_size){
    long int rc;
    rc = snd_pcm_readi(cap_handle, cap_buffer, cap_period_size);
    if (rc == -EPIPE) {
        /* EPIPE means overrun */
        fprintf(stderr, "overrun occurred\n");
        snd_pcm_prepare(cap_handle);
    } else if (rc < 0) {
        fprintf(stderr,
                "error from read: %s\n",
                snd_strerror(rc));
    } else if (rc != (int)cap_period_size) {
        fprintf(stderr, "short read, read %ld frames\n", rc);
    }
}