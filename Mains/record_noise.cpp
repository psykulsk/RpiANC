
#include <iostream>
#include <fstream>
#include <numeric>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"

#define DEPLOYED_ON_RPI

int main() {

    std::ifstream noise_file("gaussianwhitenoise.raw", std::ios::binary);
    assert(noise_file.is_open());

    std::vector<fixed_sample_type> record_vec;
    std::vector<fixed_sample_type> left_channel_noise;
    std::vector<fixed_sample_type> right_channel_noise;


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
    fixed_sample_type capture_buffer[buffer_length];


    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_device_buffer = 1024;
    snd_pcm_uframes_t play_period_size = 256;

    init_playback(&play_handle, &play_freq, &play_period_size,
                  &play_device_buffer, number_of_channels, playback_device_name);
    snd_pcm_uframes_t play_buffer_length = play_period_size * number_of_channels;
    fixed_sample_type play_buffer[play_buffer_length];

    int sample = 0;
    while (sample < 3000) {
        ++sample;
        size_t size = play_buffer_length * sizeof(fixed_sample_type);
        noise_file.read((char *) play_buffer, size);
        playback(play_handle, play_buffer, cap_period_size);
        for (unsigned long i = 0; i < buffer_length; ++i) {
            if(i%2)
                left_channel_noise.push_back(play_buffer[i]);
            else
                right_channel_noise.push_back(play_buffer[i]);
        }
        capture(cap_handle, capture_buffer, cap_period_size);
        for (unsigned long i = 0; i < buffer_length; ++i) {
            if(i%2)
                record_vec.push_back(capture_buffer[i]);
        }
    }

    save_vector_to_file("rec/recorded_whitenoise.dat", record_vec);
    save_vector_to_file("rec/left_channel_noise.dat", left_channel_noise);
    save_vector_to_file("rec/right_channel_noise.dat", right_channel_noise);
    noise_file.close();
    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

