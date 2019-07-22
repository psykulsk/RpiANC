
#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"
#include "../Headers/common.h"

#define DEPLOYED_ON_RPI
#define FEEDFORWARD

int main() {

    omp_set_num_threads(4);

    const long RESERVED_SAMPLES = 2560000;
    std::vector<fixed_sample_type> err_vec;
    err_vec.reserve(RESERVED_SAMPLES);
    std::vector<fixed_sample_type> ref_vec;
    ref_vec.reserve(RESERVED_SAMPLES);
    std::vector<fixed_sample_type> corr_vec;
    corr_vec.reserve(RESERVED_SAMPLES);
    std::vector<fixed_sample_type> corr_vec_2;
    corr_vec_2.reserve(RESERVED_SAMPLES);

#ifdef DEPLOYED_ON_RPI
    const std::string capture_device_name = "hw:CARD=sndrpisimplecar,DEV=0";
    const std::string playback_device_name = "plughw:CARD=ALSA,DEV=0";
#else
    const std::string capture_device_name = "default";
    const std::string playback_device_name = "default";
#endif

    snd_pcm_t *cap_handle;
    unsigned int play_freq = 44100;
    unsigned int number_of_channels = NR_OF_CHANNELS;
    snd_pcm_uframes_t cap_frames_per_period = CAP_FRAMES_PER_PERIOD;
    snd_pcm_uframes_t cap_frames_per_device_buffer = CAP_PERIODS_PER_BUFFER * CAP_FRAMES_PER_PERIOD;

    init_capture(&cap_handle, &play_freq, &cap_frames_per_period, &cap_frames_per_device_buffer, NR_OF_CHANNELS,
                 capture_device_name);

    snd_pcm_t *play_handle;
    snd_pcm_uframes_t play_frames_per_period = PLAY_FRAMES_PER_PERIOD;
    snd_pcm_uframes_t play_frames_per_device_buffer = PLAY_PERIODS_PER_BUFFER * PLAY_FRAMES_PER_PERIOD;

    init_playback(&play_handle, &play_freq, &play_frames_per_period,
                  &play_frames_per_device_buffer, number_of_channels, playback_device_name);

    int sample = 0;
    std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> capture_buffer = {0};
    std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> playback_buffer = {0};
    std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> processing_buffer = {0};
    const int START_PROCESSING_AFTER_SAMPLE = 1000;

    while (sample < 1200000) {
        ++sample;
#pragma omp parallel sections
        {
#pragma omp section
            {
                capture(cap_handle, capture_buffer.data(), CAP_FRAMES_PER_PERIOD);
                dc_removal(capture_buffer.data(), BUFFER_SAMPLE_SIZE);
                for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i)
                    if (i % 2)
                        err_vec.push_back(capture_buffer[i]);
                    else
                        ref_vec.push_back(capture_buffer[i]);
            }
#pragma omp section
            {
                if (sample > START_PROCESSING_AFTER_SAMPLE) {
#ifdef FEEDFORWARD
                    processing_feedforward_anc(processing_buffer.data(), BUFFER_SAMPLE_SIZE);
#else
                    processing_feedback_anc_sec_path_modelling(processing_buffer, BUFFER_SAMPLE_SIZE);
#endif
                }
                for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i){
                    if (i % 2)
                        corr_vec_2.push_back(processing_buffer[i]);
                    else
                        corr_vec.push_back(processing_buffer[i]);
                }

            }
#pragma omp section
            {
                playback(play_handle, playback_buffer.data(), PLAY_FRAMES_PER_PERIOD);
            }
        }
// Exchange data between arrays
        std::copy(processing_buffer.begin(), processing_buffer.end(), playback_buffer.begin());
        std::copy(capture_buffer.begin(), capture_buffer.end(), processing_buffer.begin());
    }

#ifdef FEEDFORWARD
    save_vector_to_file("rec/err_mic.dat", err_vec);
    save_vector_to_file("rec/ref_mic.dat", ref_vec);
    save_vector_to_file("rec/corr_sig.dat", corr_vec);
#else
    save_vector_to_file("rec/err_mic_left_channel.dat", err_vec);
    save_vector_to_file("rec/err_mic_right_channel.dat", ref_vec);
    save_vector_to_file("rec/corr_sig_left_channel.dat", corr_vec);
    save_vector_to_file("rec/corr_sig_right_channel.dat", corr_vec_2);
#endif
    snd_pcm_drain(play_handle);
    snd_pcm_close(play_handle);

    return 0;
}

