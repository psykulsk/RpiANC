//
// Created by pitersk on 29.07.18.
//

#include "../Headers/FIRFilter.h"


sample_type FIRFilter::fir_step(sample_type new_sample) {
    sample_type new_val = 0;

    // Shift sample_buffer (FIFO style)
    for (long unsigned int i = FILTER_LENGTH - 1; i >= 1; --i) {
        _samples_buffer[i] = _samples_buffer[i - 1];
    }
    _samples_buffer[0] = new_sample;
    // Multiply and accumulate
    for (long unsigned int i = 0; i < FILTER_LENGTH; ++i) {
        new_val += _samples_buffer[i] * _filter_coefficients[i];
    }
    return new_val;
}
