
#include <iostream>
#include <fstream>
#include <vector>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "Fir1fixed.h"
#include "../Headers/processing.h"

#define DEPLOYED_ON_RPI

int main() {

    std::ofstream ref_mic;
    std::ofstream err_mic;
    std::ofstream corr_sig;

    ref_mic.open("rec/ref_mic.dat", std::ios::binary);
    assert(ref_mic.is_open());
    err_mic.open("rec/err_mic.dat", std::ios::binary);
    assert(err_mic.is_open());
    corr_sig.open("rec/corr_sig.dat", std::ios::binary);
    assert(corr_sig.is_open());

    std::vector<fixed_sample_type> err_vec;
    std::vector<fixed_sample_type> ref_vec;
    std::vector<fixed_sample_type> corr_vec;

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
    snd_pcm_uframes_t play_period_size = 64;

    init_playback(&play_handle, &play_freq, &play_period_size,
                  &play_buffer_size, number_of_channels, playback_device_name);


//    Fir1fixed fir_left("../coeff12bit.dat", 12);
//    Fir1fixed fir_right("../coeff12bit.dat", 12);

    int sample = 0;

    while (sample < 10000) {
        ++sample;
        capture(cap_handle, buffer, cap_period_size);
        for (int i = 0; i < buffer_length; ++i)
            if (i % 2)
                err_vec.push_back(buffer[i]);
            else
                ref_vec.push_back(buffer[i]);
        processing(buffer, buffer_length);
        for (int i = 0; i < buffer_length; ++i)
            if (i % 2)
                corr_vec.push_back(buffer[i]);
//        for(int i=0; i < buffer_length; ++i)
//            outfile2 << buffer[i] << std::endl;
        playback(play_handle, buffer, cap_period_size);
    }

    for(fixed_sample_type s : ref_vec)
        ref_mic.write((char*)&s, sizeof(fixed_sample_type));
    for(fixed_sample_type s : err_vec)
        err_mic.write((char*)&s, sizeof(fixed_sample_type));
    for(fixed_sample_type s : corr_vec)
        corr_sig.write((char*)&s, sizeof(fixed_sample_type));

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);
    corr_sig.close();
    ref_mic.close();
    err_mic.close();

    return 0;
}

