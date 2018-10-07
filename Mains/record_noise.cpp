
#include <iostream>
#include <fstream>
#include <numeric>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"

#define DEPLOYED_ON_RPI

int main() {

    std::ifstream noise_file("whitenoise.raw", std::ios::binary);
    assert(noise_file.is_open());

    std::ofstream recorded_noise_file("recorded_whitenoise.raw", std::ios::binary);
    assert(recorded_noise_file.is_open());

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
    fixed_sample_type play_buffer[buffer_length];
    fixed_sample_type capture_buffer[buffer_length];


    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_device_buffer = 2048;
    snd_pcm_uframes_t play_period_size = 32;

    init_playback(&play_handle, &play_freq, &play_period_size,
                  &play_device_buffer, number_of_channels, playback_device_name);

    int sample = 0;
    while (sample < 10000) {
        ++sample;
        size_t size = buffer_length * sizeof(fixed_sample_type);
        noise_file.read((char *) play_buffer, size);
        playback(play_handle, play_buffer, cap_period_size);
        capture(cap_handle, capture_buffer, cap_period_size);
        fixed_sample_type record_buffer[buffer_length / 2];
        for (int i = 0; i < buffer_length; i += 2) {
            record_buffer[i / 2] = capture_buffer[i];
        }
        recorded_noise_file.write((char *) record_buffer, size / 2);
    }

    noise_file.close();
    recorded_noise_file.close();
    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

