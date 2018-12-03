//
// Created by pitersk on 07.06.18.
//

#include "../Headers/processing.h"
#include "../Headers/FxLMSFilter.h"
#include "../Headers/constants.h"

void processing_feedforward_anc(fixed_sample_type *samples_buffer,
                                long unsigned int buffer_length) {

    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(LMS_STEP_SIZE,
                                                                     FX_FILTER_COEFFS);

    for (unsigned long i = 1; i < buffer_length; i += 2) {
        // DC_REDUCTION_VALUE added to correct the dc offset in error which causes instability
        sample_type error_sample = signed_fixed_to_floating(samples_buffer[i]) + DC_REDUCTION_VALUE;
        sample_type reference_sample = signed_fixed_to_floating(samples_buffer[i - 1]);
        sample_type correction_sample = fxlms_filter.lms_step(reference_sample, error_sample);
        fixed_sample_type fixed_correction_sample = floating_to_signed_fixed(
                correction_sample * OUTPUT_GAIN);
        samples_buffer[i] = fixed_correction_sample;
        samples_buffer[i - 1] = fixed_correction_sample;
    }
}

void processing_feedback_anc(fixed_sample_type *samples_buffer, long unsigned int buffer_length) {

    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter_right_channel(LMS_STEP_SIZE,
                                                                                   FX_FILTER_COEFFS);
    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter_left_channel(LMS_STEP_SIZE,
                                                                                  FX_FILTER_COEFFS);
    static FIRFilter<FX_FILTER_LENGTH> sec_path_filter_right_channel(FX_FILTER_COEFFS);
    static FIRFilter<FX_FILTER_LENGTH> sec_path_filter_left_channel(FX_FILTER_COEFFS);
    static std::vector<fixed_sample_type> output_samples(buffer_length, 0);

    for (unsigned long i = 1; i < buffer_length; i += 2) {
        sample_type error_sample_right_channel =
                signed_fixed_to_floating(samples_buffer[i]) + DC_REDUCTION_VALUE;
        sample_type error_sample_left_channel =
                signed_fixed_to_floating(samples_buffer[i - 1]) + DC_REDUCTION_VALUE;
        sample_type reference_sample_right_channel = error_sample_right_channel +
                                                     sec_path_filter_right_channel.fir_step(
                                                             signed_fixed_to_floating(
                                                                     output_samples[i]));
        sample_type reference_sample_left_channel = error_sample_left_channel +
                                                    sec_path_filter_left_channel.fir_step(
                                                            signed_fixed_to_floating(
                                                                    output_samples[i - 1]));
        sample_type correction_sample_right_channel = fxlms_filter_right_channel.lms_step(
                reference_sample_right_channel, error_sample_right_channel);
        sample_type correction_sample_left_channel = fxlms_filter_left_channel.lms_step(
                reference_sample_left_channel, error_sample_left_channel);
        output_samples[i] = floating_to_signed_fixed(correction_sample_right_channel);
        output_samples[i - 1] = floating_to_signed_fixed(correction_sample_left_channel);
        fixed_sample_type fixed_correction_sample_right_channel = floating_to_signed_fixed(
                correction_sample_right_channel * OUTPUT_GAIN);
        fixed_sample_type fixed_correction_sample_left_channel = floating_to_signed_fixed(
                correction_sample_left_channel * OUTPUT_GAIN);
        samples_buffer[i] = fixed_correction_sample_right_channel;
        samples_buffer[i - 1] = fixed_correction_sample_left_channel;
    }
}

