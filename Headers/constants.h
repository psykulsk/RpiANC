//
// Created by pitersk on 23.10.18.
//

#ifndef RPIANC_CONSTANTS_H
#define RPIANC_CONSTANTS_H


#include "common.h"
#include "FxLMSFilter.h"

const long unsigned int FILTER_LENGTH = 35;
const long unsigned int FX_FILTER_LENGTH = 4;
const sample_type LMS_STEP_SIZE = 0.001f;
const sample_type SEC_PATH_LMS_STEP_SIZE = 0.000001f;
const float DC_REDUCTION_VALUE = 0.056f;
const float INPUT_SCALING = 1.0f;
const float OUTPUT_GAIN = 1.0f;
const FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array FX_FILTER_COEFFS = {
        0.0f, 0.0f, 0.0f,
        1.0f
};
//const FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array FX_FILTER_COEFFS = {
//        1.0f
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        0.0f, 0.0f, 0.0f,
//        1.0f
//};


#endif //RPIANC_CONSTANTS_H
