//
// Created by pitersk on 29.07.18.
//

#include "../Headers/LMSFilter.h"

sample_type LMSFilter::lms_step(sample_type x_reference_sample, sample_type error_sample) {
    // Update filter coefficients
    fir_filter.lms_coefficients_update(-_alpha * error_sample);
    // Perform filtering step, to generate new y correction sample
    return fir_filter.fir_step(x_reference_sample);
}
