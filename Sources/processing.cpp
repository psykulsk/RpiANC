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
//    static FxLMSFilter<1, FILTER_LENGTH>::fx_filter_coeffs_array s_filter_coeffs = {
//            1.0
//    };
    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(3.0, s_filter_coeffs);
//    LMSFilter<FX_FILTER_LENGTH> lms_filter(0.5);

    for (unsigned long i = 1; i < buffer_length; i += 2) {
        sample_type error_sample = signed_fixed_to_floating(samples_buffer[i]);
        sample_type reference_sample = signed_fixed_to_floating(samples_buffer[i - 1]);
        sample_type correction_sample = fxlms_filter.lms_step(reference_sample, error_sample);
        fixed_sample_type fixed_correction_sample = floating_to_signed_fixed(correction_sample);
        samples_buffer[i] = fixed_correction_sample;
        samples_buffer[i - 1] = fixed_correction_sample;
    }
}

sample_type firFilt(sample_type new_sample,
                    const sample_type (&filter_coefficients)[FILTER_LENGTH]) {
    static sample_type sample_buffer[FILTER_LENGTH] = {0};
    sample_type new_val = 0;

    // Shift sample_buffer (FIFO style)
    for (long unsigned int i = FILTER_LENGTH - 1; i >= 1; --i) {
        sample_buffer[i] = sample_buffer[i - 1];
    }
    sample_buffer[0] = new_sample;
    // Multiply and accumulate
    for (long unsigned int i = 0; i < FILTER_LENGTH; ++i) {
        new_val += sample_buffer[i] * filter_coefficients[i];
    }
    return new_val;
}


