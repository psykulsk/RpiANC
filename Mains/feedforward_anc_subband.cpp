
#include <vector>
#include <chrono>
#include <iostream>
#include <omp.h>
#include "../Headers/capture.h"
#include "../Headers/playback.h"
#include "../Headers/processing.h"
#include "../Headers/common.h"
#include "../Headers/subband_filtering_constants.h"
#include "../Headers/constants.h"
#include "../Headers/FIRFilter.h"

#define DEPLOYED_ON_RPI
#define FEEDFORWARD

typedef std::array<fixed_sample_type, BUFFER_SAMPLE_SIZE> fixed_samples_buffer;
typedef std::array<sample_type, SUB_BUFFER_SIZE> samples_sub_buffer;
typedef std::array<samples_sub_buffer, NR_OF_SUBBANDS> subband_buffers;
typedef FIRFilter<SUBBAND_FILTER_LENGTH> subband_filter;
typedef std::array<subband_filter, NR_OF_SUBBANDS> subband_filters;

void subband_analysis(fixed_samples_buffer &main_buffer, subband_buffers &sub_buffers) {
    static subband_filters error_filters = {
            subband_filter(ANALYSIS_COEFFS_1),
            subband_filter(ANALYSIS_COEFFS_2),
            subband_filter(ANALYSIS_COEFFS_3),
            subband_filter(ANALYSIS_COEFFS_4)
    };
    static subband_filters reference_filters = {
            subband_filter(ANALYSIS_COEFFS_1),
            subband_filter(ANALYSIS_COEFFS_2),
            subband_filter(ANALYSIS_COEFFS_3),
            subband_filter(ANALYSIS_COEFFS_4)
    };

    for (unsigned long sample_i = 1; sample_i < BUFFER_SAMPLE_SIZE-NR_OF_SUBBANDS; sample_i += 2) {
        sample_type error_sample = signed_fixed_to_floating(main_buffer[sample_i]);
        sample_type reference_sample = signed_fixed_to_floating(main_buffer[sample_i - 1]);
        for (unsigned long subband_i = 0; subband_i < NR_OF_SUBBANDS; ++subband_i) {
            sample_type new_error_sample = error_filters.at(subband_i).fir_step(error_sample);
            sample_type new_ref_sample = reference_filters.at(subband_i).fir_step(reference_sample);
            // Downsample
            if (sample_i % NR_OF_SUBBANDS == 1) {
                sub_buffers.at(subband_i).at(sample_i / 4 + 1) = new_error_sample;
                sub_buffers.at(subband_i).at(sample_i / 4) = new_ref_sample;
            }
        }
    }

}

void subband_synthesis(fixed_samples_buffer &main_buffer, subband_buffers &buffers) {
    static subband_filters filters = {
            subband_filter(SYNTHESIS_COEFFS_1),
            subband_filter(SYNTHESIS_COEFFS_2),
            subband_filter(SYNTHESIS_COEFFS_3),
            subband_filter(SYNTHESIS_COEFFS_4)
    };

    for (unsigned long sample_i = 1; sample_i < BUFFER_SAMPLE_SIZE; sample_i += 2) {
        float synthesized_sample = 0.0f;
        for (unsigned long subband_i = 0; subband_i < NR_OF_SUBBANDS; ++subband_i) {
            sample_type new_ref_sample;
            // Upsample with 0s
            if (sample_i % NR_OF_SUBBANDS == 1) {
                sample_type sample = buffers.at(subband_i).at(sample_i / 4);
                new_ref_sample = filters.at(subband_i).fir_step(sample);
            } else {
                new_ref_sample = filters.at(subband_i).fir_step(0.0f);
            }
            synthesized_sample += new_ref_sample;
        }
//      Fill both output channels
        main_buffer.at(sample_i) = floating_to_signed_fixed(synthesized_sample);
        main_buffer.at(sample_i - 1) = floating_to_signed_fixed(synthesized_sample);
    }

}

int main() {

    omp_set_num_threads(8);
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
    subband_buffers sub_buffers = {{0}};
    const int START_PROCESSING_AFTER_SAMPLE = 1000;

    while (sample < 12000) {
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
                    subband_analysis(processing_buffer, sub_buffers);
#pragma omp parallel for
                    for (unsigned int subband_i = 0; subband_i < NR_OF_SUBBANDS; ++subband_i) {
                        processing_feedforward_anc_subband(sub_buffers.at(subband_i).data(), BUFFER_SAMPLE_SIZE);
                    }
                    subband_synthesis(processing_buffer, sub_buffers);
#else
                    processing_feedback_anc_sec_path_modelling(processing_buffer, BUFFER_SAMPLE_SIZE);
#endif
                }
                for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i) {
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

