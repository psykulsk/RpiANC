
#include "../Headers/capture.h"
#include "../Headers/playback.h"


int main() {

//    const std::string capture_device_name = "default";
//    const std::string playback_device_name = "default";

    const std::string capture_device_name = "plughw:CARD=sndrpisimplecar,DEV=0";
    const std::string playback_device_name = "plughw:CARD=ALSA,DEV=0";

    int rc = 0;
    snd_pcm_uframes_t size;
    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = 2;
    snd_pcm_uframes_t cap_period_size = 64;

    init_capture(&cap_handle, &play_freq, &cap_period_size, number_of_channels,
                 capture_device_name);
    snd_pcm_uframes_t length = cap_period_size*number_of_channels;
    size = length * sizeof(cap_sample_type);
    cap_sample_type buffer[length];


    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_buffer_size = 1024;
    snd_pcm_uframes_t play_period_size = 128;

    init_playback(&play_handle, &play_freq, &play_period_size,
                  &play_buffer_size, number_of_channels, playback_device_name);

    snd_pcm_uframes_t play_length = play_period_size*number_of_channels;
    snd_pcm_uframes_t play_size = play_length * sizeof(play_sample_type);
    play_sample_type play_buffer[play_length];

    while (true) {
        capture(cap_handle, buffer, cap_period_size);



        playback(play_handle, buffer, cap_period_size);



    }

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

