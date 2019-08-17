
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
    static FIRFilter<ANTYALIAS_FILTER_LENGTH> aa_filter_error(ANTYALIAS_FILTER_COEFFS);
    static FIRFilter<ANTYALIAS_FILTER_LENGTH> aa_filter_ref(ANTYALIAS_FILTER_COEFFS);

    for (unsigned long sample_i = 1; sample_i < BUFFER_SAMPLE_SIZE - NR_OF_SUBBANDS; sample_i += 2) {
        sample_type error_sample = signed_fixed_to_floating(main_buffer[sample_i]);
        error_sample = aa_filter_error.fir_step(error_sample);
        sample_type reference_sample = signed_fixed_to_floating(main_buffer[sample_i - 1]);
        reference_sample = aa_filter_ref.fir_step(reference_sample);
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

    static FIRFilter<ANTYALIAS_FILTER_LENGTH> aa_filter(ANTYALIAS_FILTER_COEFFS);

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
        sample_type out_sample = aa_filter.fir_step(synthesized_sample);
        main_buffer.at(sample_i) = floating_to_signed_fixed(out_sample);
        main_buffer.at(sample_i - 1) = floating_to_signed_fixed(out_sample);
    }

}

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
    subband_buffers subband_buffers_cap = {0.0f};
    subband_buffers subband_buffers_proc = {0.0f};
    subband_buffers subband_buffers_play = {0.0f};
    const int START_PROCESSING_AFTER_SAMPLE = 10;

    while (sample < 52000) {
        ++sample;
#pragma omp parallel sections
        {
#pragma omp section
            {
                capture(cap_handle, capture_buffer.data(), CAP_FRAMES_PER_PERIOD);
                dc_removal(capture_buffer.data(), BUFFER_SAMPLE_SIZE);
                subband_analysis(capture_buffer, subband_buffers_cap);
                for (unsigned int i = 0; i < BUFFER_SAMPLE_SIZE; ++i)
                    if (i % 2)
                        err_vec.push_back(capture_buffer[i]);
                    else
                        ref_vec.push_back(capture_buffer[i]);
            }
#pragma omp section
            {
                if (sample > START_PROCESSING_AFTER_SAMPLE) {
//#pragma omp parallel for
                    for (unsigned int subband_i = 0; subband_i < NR_OF_SUBBANDS; ++subband_i) {
                        processing_feedforward_anc_subband(subband_buffers_proc.at(subband_i).data(),
                                                           SUB_BUFFER_SIZE, subband_i);
                    }
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
                subband_synthesis(playback_buffer, subband_buffers_play);
                playback(play_handle, playback_buffer.data(), PLAY_FRAMES_PER_PERIOD);
            }
        }
// Exchange data between arrays
#pragma omp parallel for
        for (unsigned int i = 0; i < NR_OF_SUBBANDS; ++i) {
            std::copy(subband_buffers_proc.at(i).begin(), subband_buffers_proc.at(i).end(),
                      subband_buffers_play.at(i).begin());
        }
#pragma omp parallel for
        for (unsigned int i = 0; i < NR_OF_SUBBANDS; ++i) {
            std::copy(subband_buffers_cap.at(i).begin(), subband_buffers_cap.at(i).end(),
                      subband_buffers_proc.at(i).begin());
        }
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

