#include <iostream>
#include <fstream>
#include <vector>
#include "../Headers/capture.h"

#define DEPLOYED_ON_RPI

int main() {

    std::vector<fixed_sample_type> err_vec;
    std::vector<fixed_sample_type> ref_vec;

#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = RPI_CAPTURE_DEVICE_NAME;
#else
    const std::string capture_device_name = "default";
#endif


    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = NR_OF_CHANNELS;
    snd_pcm_uframes_t cap_frames_per_period = CAP_FRAMES_PER_PERIOD;
    snd_pcm_uframes_t cap_frames_per_device_buffer = CAP_PERIODS_PER_BUFFER * CAP_FRAMES_PER_PERIOD;

    init_capture(&cap_handle, &play_freq, &cap_frames_per_period, &cap_frames_per_device_buffer,number_of_channels,
                 capture_device_name);
    fixed_sample_type buffer[BUFFER_SAMPLE_SIZE];

    int sample = 0;
    while (sample < 10000) {
        ++sample;
        capture(cap_handle, buffer, BUFFER_SAMPLE_SIZE);
        for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i)
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

