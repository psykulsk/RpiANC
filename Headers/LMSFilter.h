//
// Created by pitersk on 29.07.18.
//

#ifndef RPIANC_LMSFILTER_H
#define RPIANC_LMSFILTER_H


#include "common.h"
#include "FIRFilter.h"

class LMSFilter {

public:
    FIRFilter fir_filter;

    LMSFilter(float alpha_val) : _alpha{alpha_val}, _lms_coefficients{{0}} {

    }

    sample_type lms_step(sample_type x_reference_sample, sample_type error_sample);

private:
    float _alpha;
    filter_coeffs_array _lms_coefficients;
};


#endif //RPIANC_LMSFILTER_H
