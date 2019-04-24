//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_PROCESSING_H
#define RPIANC_PROCESSING_H


#include "common.h"
#include "../Headers/constants.h"

void dc_removal(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedforward_anc(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedforward_anc_subband(sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedforward_anc_sec_path_modelling(fixed_sample_type *samples_buffer,
                                                   long unsigned int buffer_length);

void processing_feedback_anc(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedback_anc_sec_path_modelling(fixed_sample_type *samples_buffer,
                                                long unsigned int buffer_length);


#endif //RPIANC_PROCESSING_H
