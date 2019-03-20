//
// Created by pitersk on 23.10.18.
//

#ifndef RPIANC_CONSTANTS_H
#define RPIANC_CONSTANTS_H


#include "common.h"

const size_t NR_OF_CHANNELS = 2;
const long unsigned int FILTER_LENGTH = 256;
const long unsigned int FX_FILTER_LENGTH = 7;
const sample_type LMS_STEP_SIZE = 0.1f;
const sample_type SEC_PATH_LMS_STEP_SIZE = 0.000001f;
const float DC_REDUCTION_VALUE = 0.056f;
const float INPUT_SCALING = 1.0f;
const float OUTPUT_GAIN = 1.0f;
const std::array<sample_type, FX_FILTER_LENGTH> FX_FILTER_COEFFS = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        1.0f
};

#endif //RPIANC_CONSTANTS_H
