//
// Created by pitersk on 23.10.18.
//

#ifndef RPIANC_CONSTANTS_H
#define RPIANC_CONSTANTS_H


#include "common.h"

const size_t NR_OF_CHANNELS = 2;
const long unsigned int FILTER_LENGTH = 256;
const long unsigned int FX_FILTER_LENGTH = 7;
const sample_type LMS_STEP_SIZE = 0.001f;
const sample_type SEC_PATH_LMS_STEP_SIZE = 0.000001f;
const float DC_REDUCTION_VALUE = 0.0504f;
const float INPUT_SCALING = 1.0f;
const float OUTPUT_GAIN = 2.0f;
const std::array<sample_type, FX_FILTER_LENGTH> FX_FILTER_COEFFS = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f
};

const size_t PLAY_FRAMES_PER_PERIOD = 128;
const size_t PLAY_PERIODS_PER_BUFFER = 8;
const size_t CAP_FRAMES_PER_PERIOD = 128;
const size_t CAP_PERIODS_PER_BUFFER = 8;
const size_t BUFFER_SAMPLE_SIZE = CAP_FRAMES_PER_PERIOD*NR_OF_CHANNELS;

#endif //RPIANC_CONSTANTS_H
