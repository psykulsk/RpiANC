//
// Created by pitersk on 07.06.18.
//

#include "../Headers/processing.h"
#include "../Headers/FxLMSFilter.h"
#include "../Headers/constants.h"

void processing(fixed_sample_type *samples_buffer, long unsigned int buffer_length) {

    static FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(LMS_STEP_SIZE,
                                                                     FX_FILTER_COEFFS);

    for (unsigned long i = 1; i < buffer_length; i += 2) {
        // DC_REDUCTION_VALUE added to correct the dc offset in error which causes instability
        sample_type error_sample = signed_fixed_to_floating(samples_buffer[i]) + DC_REDUCTION_VALUE;
        sample_type reference_sample = signed_fixed_to_floating(samples_buffer[i - 1]);
        sample_type correction_sample = fxlms_filter.lms_step(reference_sample, error_sample);
        fixed_sample_type fixed_correction_sample = floating_to_signed_fixed(correction_sample*OUTPUT_GAIN);
        samples_buffer[i] = fixed_correction_sample;
        samples_buffer[i - 1] = fixed_correction_sample;
    }
}


