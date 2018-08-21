//
// Created by pitersk on 21.08.18.
//

#ifndef RPIANC_FXLMSFILTER_H
#define RPIANC_FXLMSFILTER_H

#include "LMSFilter.h"
#include "FIRFilter.h"

class FxLMSFilter: public LMSFilter {

    FxLMSFilter(float alpha_val, const filter_coeffs_array s_filter) : LMSFilter(alpha_val), _s_filter{s_filter} {};

    sample_type lms_step(sample_type x_reference_sample, sample_type error_sample);

private:
    FIRFilter _s_filter;

};


#endif //RPIANC_FXLMSFILTER_H
