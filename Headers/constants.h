//
// Created by pitersk on 23.10.18.
//

#ifndef RPIANC_CONSTANTS_H
#define RPIANC_CONSTANTS_H


#include "common.h"
#include "FxLMSFilter.h"

const long unsigned int FILTER_LENGTH = 30;
const long unsigned int FX_FILTER_LENGTH = 5;
const sample_type LMS_STEP_SIZE = 0.1f;
const float DC_REDUCTION_VALUE = 0.05f;
const float OUTPUT_GAIN = 5.0f;
const FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array FX_FILTER_COEFFS = {
        -0.0121146f,
        -0.0072095f,
        -0.0046806f,
        0.0041966f,
        0.0237970f
};
#endif //RPIANC_CONSTANTS_H
