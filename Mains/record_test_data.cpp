#include <iostream>
#include <fstream>
#include <vector>
#include "../Headers/capture.h"

#define DEPLOYED_ON_RPI

int main() {

    std::vector<fixed_sample_type> err_vec;
    std::vector<fixed_sample_type> ref_vec;

#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = "hw:CARD=sndrpisimplecar,DEV=0";
#else
    const std::string capture_device_name = "default";
#endif


    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = 2;
    snd_pcm_uframes_t cap_period_size = 64;

    init_capture(&cap_handle, &play_freq, &cap_period_size, number_of_channels,
                 capture_device_name);
    snd_pcm_uframes_t buffer_length = cap_period_size * number_of_channels;
    fixed_sample_type buffer[buffer_length];

    int sample = 0;
    while (sample < 10000) {
        ++sample;
        capture(cap_handle, buffer, cap_period_size);
        for (unsigned int i = 0; i < buffer_length; ++i)
            if (i % 2)
                err_vec.push_back(buffer[i]);
            else
                ref_vec.push_back(buffer[i]);
    }

    save_vector_to_file("rec/ref_mic.dat", ref_vec);
    save_vector_to_file("rec/err_mic.dat", err_vec);

    snd_pcm_drain(cap_handle);
    snd_pcm_close(cap_handle);
    return 0;
}

