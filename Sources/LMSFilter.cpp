//
// Created by pitersk on 29.07.18.
//

#include "../Headers/LMSFilter.h"

sample_type LMSFilter::lms_step(sample_type x_reference_sample, sample_type error_sample) {
    // Shift samples buffer
    for (long unsigned int i = FILTER_LENGTH - 1; i >= 1; --i) {
        _samples_buffer[i] = _samples_buffer[i - 1];
    }
    _samples_buffer[0] = x_reference_sample;
    // Update filter coefficients
    lms_filter_update(-_alpha * static_cast<double>(error_sample));
    // Perform filtering step, to generate new y correction sample
    return fir_filter.fir_step(x_reference_sample);
}

void LMSFilter::lms_filter_update(double update_step) {
    filter_coeffs_array filter_coeffs = fir_filter.get_coefficients();
    for (int i = 0; i < FILTER_LENGTH; ++i) {
        filter_coeffs.at(i) += _samples_buffer.at(i) * update_step;
    }
    fir_filter.set_coefficients(filter_coeffs);
}


