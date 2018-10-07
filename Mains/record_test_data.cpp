#include <iostream>
#include <fstream>
#include <vector>
#include "../Headers/capture.h"

#define DEPLOYED_ON_RPI

int main() {

    std::ofstream ref_mic;
    std::ofstream err_mic;

    ref_mic.open("rec/ref_mic_test.dat", std::ios::binary);
    err_mic.open("rec/err_mic_test.dat", std::ios::binary);
    assert(ref_mic.is_open());
    assert(err_mic.is_open());
    std::vector<fixed_sample_type> err_vec;
    std::vector<fixed_sample_type> ref_vec;

#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = "plughw:CARD=sndrpisimplecar,DEV=0";
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
        for (int i = 0; i < buffer_length; ++i)
            if (i % 2)
                err_vec.push_back(buffer[i]);
            else
                ref_vec.push_back(buffer[i]);
    }

    for (fixed_sample_type s : ref_vec)
        ref_mic.write((char *) &s, sizeof(fixed_sample_type));
    for (fixed_sample_type s : err_vec)
        err_mic.write((char *) &s, sizeof(fixed_sample_type));

    snd_pcm_drain(cap_handle);
    snd_pcm_close(cap_handle);
    ref_mic.close();
    err_mic.close();

    return 0;
}

