//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_PROCESSING_H
#define RPIANC_PROCESSING_H


#include "common.h"
#include "../Headers/constants.h"
#include "../Headers/LMSFilter.h"

void secondary_path_identification(fixed_sample_type *samples_buffer,fixed_sample_type *ref_for_processing,
                                   long unsigned int buffer_length,
                                   LMSFilter<FX_FILTER_LENGTH> &sec_path_estimation_filter);

void dc_removal(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedforward_anc(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedforward_anc_subband(sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedforward_anc_sec_path_modelling(fixed_sample_type *samples_buffer,
                                                   long unsigned int buffer_length);

void processing_feedback_anc(fixed_sample_type *samples_buffer, long unsigned int buffer_length);

void processing_feedback_anc_sec_path_modelling(fixed_sample_type *samples_buffer,
                                                long unsigned int buffer_length);


#endif //RPIANC_PROCESSING_H
