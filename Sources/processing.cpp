//
// Created by pitersk on 07.06.18.
//

#include "../Headers/processing.h"


void
processing(sample_type *samples_buffer, long unsigned int buffer_length, Fir1fixed &fir_left, Fir1fixed &fir_right) {

    for (int i = 0; i < buffer_length; ++i) {
        if (i % 2) {
//            samples_buffer[i] = fir_left.filter(samples_buffer[i]);
        } else {

//            samples_buffer[i] = fir_right.filter(samples_buffer[i]);
            //         *(samples_buffer+i) = 30000;
        }
    }

}

sample_type firFilt(sample_type new_sample, const sample_type (&filter_coefficients)[FILTER_LENGTH]) {
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


sample_type lms_step(sample_type x_sample, sample_type d_sample, float step_size) {

}
