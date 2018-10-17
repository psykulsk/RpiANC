//
// Created by pitersk on 07.06.18.
//

#include "../Headers/processing.h"
#include "../Headers/FxLMSFilter.h"

void processing(fixed_sample_type *samples_buffer, long unsigned int buffer_length) {
    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array s_filter_coeffs = {
            -0.0121146,
            -0.0072095,
            -0.0046806,
            0.0041966,
            0.0237970
    };

    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(0.8, s_filter_coeffs);

    for (unsigned long i = 1; i < buffer_length; i += 2) {
        // 0.05 added to correct the dc offset in error which causes instability
        sample_type error_sample = signed_fixed_to_floating(samples_buffer[i]) + 0.05;
        sample_type reference_sample = signed_fixed_to_floating(samples_buffer[i - 1]);
        sample_type correction_sample = fxlms_filter.lms_step(reference_sample, error_sample);
        fixed_sample_type fixed_correction_sample = floating_to_signed_fixed(correction_sample);
        samples_buffer[i] = fixed_correction_sample;
        samples_buffer[i - 1] = fixed_correction_sample;
    }
}


