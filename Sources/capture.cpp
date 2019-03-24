//
// Created by pitersk on 27.05.18.
//

#include "../Headers/capture.h"
#include <iostream>

void init_capture(snd_pcm_t **cap_handle, unsigned int *cap_freq, snd_pcm_uframes_t *cap_period_size,
                  snd_pcm_uframes_t *cap_buffer_size, unsigned int number_of_channels,
                  const std::string capture_device_name) {

    snd_pcm_hw_params_t *params;
    int rc;
    int dir;

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
                                 SND_PCM_FORMAT_S32_LE);

    /* Two channels (stereo) */
    snd_pcm_hw_params_set_channels(*cap_handle, params, number_of_channels);

    /* 44100 bits/second sampling rate (CD quality) */

    snd_pcm_hw_params_set_rate_near(*cap_handle, params,
                                    cap_freq, &dir);

//    snd_pcm_hw_params_set_buffer_size_near (cap_handle, params, &frames);
    snd_pcm_hw_params_set_period_size_near(*cap_handle, params, cap_period_size, NULL);
    snd_pcm_hw_params_set_buffer_size_near(*cap_handle, params, cap_buffer_size);
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

    std::cerr << "Capture params: " << "Capture rate: " << *cap_freq << " Period size: " << *cap_period_size
              << std::endl;

    if (*cap_period_size != CAP_FRAMES_PER_PERIOD) {
        std::cout << "Number of cap frames per period ( " << *cap_period_size << " ) then configuration ( "
                  << CAP_FRAMES_PER_PERIOD << " )" << std::endl;
        exit(1);
    } else if (*cap_buffer_size != CAP_FRAMES_PER_PERIOD * CAP_PERIODS_PER_BUFFER) {
        std::cout << "Number of play frames per device buffer( " << *cap_buffer_size
                  << " ) then configuration ( " << CAP_FRAMES_PER_PERIOD * CAP_PERIODS_PER_BUFFER << " )"
                  << std::endl;
        exit(1);
    }

    snd_pcm_hw_params_free(params);
    snd_pcm_prepare(*cap_handle);
}

void capture(snd_pcm_t *cap_handle, fixed_sample_type *cap_buffer,
             snd_pcm_uframes_t frames_in_cap_period) {
    long int rc;
    rc = snd_pcm_readi(cap_handle, cap_buffer, frames_in_cap_period);
    if (rc == -EPIPE) {
        /* EPIPE means overrun */
        fprintf(stderr, "capture overrun occurred\n");
        snd_pcm_prepare(cap_handle);
    } else if (rc < 0) {
        fprintf(stderr,
                "error from read: %s\n",
                snd_strerror(rc));
    } else if (rc != (int) frames_in_cap_period) {
        fprintf(stderr, "short read, read %ld frames\n", rc);
    }
}