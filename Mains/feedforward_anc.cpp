
#include <vector>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"
#include "../Headers/common.h"
#define DEPLOYED_ON_RPI

int main() {

    std::vector<fixed_sample_type> err_vec;
    err_vec.reserve(1280000);
    std::vector<fixed_sample_type> ref_vec;
    ref_vec.reserve(1280000);
    std::vector<fixed_sample_type> corr_vec;
    corr_vec.reserve(1280000);

#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = "plughw:CARD=sndrpisimplecar,DEV=0";
    const std::string playback_device_name = "plughw:CARD=ALSA,DEV=0";
#else
    const std::string capture_device_name = "default";
    const std::string playback_device_name = "default";
#endif

    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = 2;
    snd_pcm_uframes_t cap_period_size = 64;

    init_capture(&cap_handle, &play_freq, &cap_period_size, number_of_channels,
                 capture_device_name);
    snd_pcm_uframes_t buffer_length = cap_period_size * number_of_channels;
    fixed_sample_type buffer[buffer_length];


    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_buffer_size = 1024;
    snd_pcm_uframes_t play_period_size = 256;

    init_playback(&play_handle, &play_freq, &play_period_size,
                  &play_buffer_size, number_of_channels, playback_device_name);

    int sample = 0;

    while (sample < 12000) {
        ++sample;
        capture(cap_handle, buffer, cap_period_size);
        for (unsigned int i = 0; i < buffer_length; ++i)
            if (i % 2)
                err_vec.push_back(buffer[i]);
            else
                ref_vec.push_back(buffer[i]);
        processing(buffer, buffer_length);
        for (unsigned int i = 0; i < buffer_length; ++i)
            if (i % 2)
                corr_vec.push_back(buffer[i]);
        playback(play_handle, buffer, cap_period_size);
    }

    save_vector_to_file("rec/err_mic.dat", err_vec);
    save_vector_to_file("rec/ref_mic.dat", ref_vec);
    save_vector_to_file("rec/corr_sig.dat", corr_vec);

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

