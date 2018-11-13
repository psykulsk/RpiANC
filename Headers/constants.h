//
// Created by pitersk on 23.10.18.
//

#ifndef RPIANC_CONSTANTS_H
#define RPIANC_CONSTANTS_H


#include "common.h"
#include "FxLMSFilter.h"

const long unsigned int FILTER_LENGTH = 40;
const long unsigned int FX_FILTER_LENGTH = 5;
const sample_type LMS_STEP_SIZE = 0.01f;
const float DC_REDUCTION_VALUE = 0.05f;
const float OUTPUT_GAIN = 1.0f;
const FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array FX_FILTER_COEFFS = {
        -5.4421e-02f,
        9.6128e-02f,
        -6.3437e-04f,
        -1.1989e-01f,
        9.4738e-02f
};

#endif //RPIANC_CONSTANTS_H
