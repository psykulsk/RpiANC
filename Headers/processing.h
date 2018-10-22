//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_PROCESSING_H
#define RPIANC_PROCESSING_H


#include <Fir1fixed.h>
#include "common.h"


void processing(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

sample_type firFilt(sample_type new_sample, const sample_type (&filter_coefficients)[FILTER_LENGTH]);
sample_type lms_step(sample_type x_sample, sample_type d_sample, float step_size);

#endif //RPIANC_PROCESSING_H
