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

    std::cerr << "Playback params: " << "Playback rate: " << *play_freq << " Period size: "
              << *play_period_size <<
              " Buffer size: " << *play_buffer_size << std::endl;


    unsigned int val, val2;
    snd_pcm_uframes_t frames;

    snd_pcm_hw_params_get_access(params,
                                 (snd_pcm_access_t *) &val);
    printf("access type = %s\n",
           snd_pcm_access_name((snd_pcm_access_t) val));

    snd_pcm_hw_params_get_subformat(params,
                                    (snd_pcm_subformat_t *) &val);
    printf("subformat = '%s' (%s)\n",
           snd_pcm_subformat_name((snd_pcm_subformat_t) val),
           snd_pcm_subformat_description(
                   (snd_pcm_subformat_t) val));


    snd_pcm_hw_params_get_channels(params, &val);
    printf("channels = %d\n", val);

    snd_pcm_hw_params_get_rate(params, &val, &dir);
    printf("rate = %d bps\n", val);

    snd_pcm_hw_params_get_period_time(params,
                                      &val, &dir);
    printf("period time = %d us\n", val);

    snd_pcm_hw_params_get_period_size(params,
                                      &frames, &dir);
    printf("period size = %d frames\nperiod size = %d bytes\n", (int) frames,
           (int) snd_pcm_frames_to_bytes(*handle, frames));

    snd_pcm_hw_params_get_buffer_time(params,
                                      &val, &dir);
    printf("buffer time = %d us\n", val);

    snd_pcm_hw_params_get_buffer_size(params,
                                      (snd_pcm_uframes_t *) &val);
    printf("buffer size = %d frames\n", val);

    snd_pcm_hw_params_get_periods(params, &val, &dir);
    printf("periods per buffer = %d frames\n", val);

    snd_pcm_hw_params_get_period_size_min(params, &frames, &dir);
    printf(" min period size = %d frames\n", (int) frames);

    snd_pcm_hw_params_get_buffer_size_min(params, &frames);
    printf(" min buffer size = %d frames\n", (int) frames);

    snd_pcm_hw_params_get_rate_numden(params,
                                      &val, &val2);
    printf("exact rate = %d/%d bps\n", val, val2);

    val = snd_pcm_hw_params_get_sbits(params);
    printf("significant bits = %d\n", val);

    val = snd_pcm_hw_params_is_batch(params);
    printf("is batch = %d\n", val);

    val = snd_pcm_hw_params_is_block_transfer(params);
    printf("is block transfer = %d\n", val);

    val = snd_pcm_hw_params_is_double(params);
    printf("is double = %d\n", val);

    val = snd_pcm_hw_params_is_half_duplex(params);
    printf("is half duplex = %d\n", val);

    val = snd_pcm_hw_params_is_joint_duplex(params);
    printf("is joint duplex = %d\n", val);

    val = snd_pcm_hw_params_can_overrange(params);
    printf("can overrange = %d\n", val);

    val = snd_pcm_hw_params_can_mmap_sample_resolution(params);
    printf("can mmap = %d\n", val);

    val = snd_pcm_hw_params_can_pause(params);
    printf("can pause = %d\n", val);

    val = snd_pcm_hw_params_can_resume(params);
    printf("can resume = %d\n", val);

    val = snd_pcm_hw_params_can_sync_start(params);
    printf("can sync start = %d\n", val);

    if (*play_period_size != PLAY_FRAMES_PER_PERIOD) {
        std::cout << "Number of play frames per period ( " << *play_period_size << " ) then configuration ( "
                  << PLAY_FRAMES_PER_PERIOD << " )" << std::endl;
        exit(1);
    } else if (*play_buffer_size != PLAY_FRAMES_PER_PERIOD * PLAY_PERIODS_PER_BUFFER) {
        std::cout << "Number of play frames per device buffer( " << *play_buffer_size
                  << " ) then configuration ( " << PLAY_FRAMES_PER_PERIOD * PLAY_PERIODS_PER_BUFFER << " )"
                  << std::endl;
        exit(1);
    }

    snd_pcm_hw_params_free(params);
    snd_pcm_prepare(*handle);
}

void playback(snd_pcm_t *play_handle, fixed_sample_type *play_buffer,
              snd_pcm_uframes_t frames_in_period) {
    ssize_t rc;
    rc = snd_pcm_writei(play_handle, play_buffer, frames_in_period);
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
    } else if (rc != (int) frames_in_period) {
        fprintf(stderr,
                "playback short write, write %ld frames\n", rc);
    }
}