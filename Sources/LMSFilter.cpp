//
// Created by pitersk on 29.07.18.
//

#include "../Headers/LMSFilter.h"

sample_type LMSFilter::lms_step(sample_type x_reference_sample, sample_type error_sample) {
    // Update filter coefficients
    update_filter(-_alpha*static_cast<float>(error_sample));
    // Perform filtering step, to generate new y correction sample
    return fir_filter.fir_step(x_reference_sample);

}

void LMSFilter::update_filter(float filter_coeff_change) {
    filter_coeffs_array filter_values = fir_filter.get_coefficients();
    for (float &coeff : filter_values) {
        coeff = coeff + filter_coeff_change;
    }
    fir_filter.set_coefficients(filter_values);
}
