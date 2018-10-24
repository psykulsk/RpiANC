
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "Fir1fixed.h"
#include "../Headers/processing.h"
#include "../Headers/LMSFilter.h"


#define DEPLOYED_ON_RPI


int main() {

    std::ifstream noise_file("whitenoise.raw", std::ios::binary);
    assert(noise_file.is_open());

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

    LMSFilter<FX_FILTER_LENGTH> lms_filter(0.0001);

    std::vector<sample_type> error_vec;
    std::vector<FIRFilter<FX_FILTER_LENGTH>::filter_coeffs_array> filter_coeffs_vec;

    int sample = 0;
    while (sample < 10000) {
        ++sample;
        size_t size = buffer_length * sizeof(fixed_sample_type);
        noise_file.read((char *) play_buffer, size);
//        rc = read(0, play_buffer, size);

//        if (rc == 0) {
//            fprintf(stderr, "end of file on input\n");
//            break;
//        } else if (rc != size) {
//            fprintf(stderr,
//                    "short read: read %d bytes\n", rc);
//        }
        playback(play_handle, play_buffer, cap_period_size);
        capture(cap_handle, capture_buffer, cap_period_size);

        std::vector<sample_type> lms_output(cap_period_size / 2, 0);
        for (unsigned int i = 0; i < cap_period_size; i += 2) {
            sample_type second_path_sample = signed_fixed_to_floating(capture_buffer[i]);
            sample_type error_sample = second_path_sample - lms_output.at(i / 2);
            lms_output.at(i / 2) = lms_filter.lms_step(second_path_sample, error_sample);
            error_vec.push_back(error_sample);
            filter_coeffs_vec.push_back(lms_filter.fir_filter.get_coefficients());
        }
    }


//    std::vector<sample_type> x(error_vec.size());
//    std::iota(x.begin(), x.end(), 0);
//    plt::subplot(3, 1, 1);
//    plt::plot(reference_signal);
//    plt::subplot(3, 1, 2);
//    plt::plot(output);
//    plt::subplot(3, 1, 3);
//    plt::semilogy(x, error_vec);
//    plt::show();

    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

