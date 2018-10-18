//
// Created by pitersk on 21.08.18.
//

#ifndef RPIANC_FXLMSFILTER_H
#define RPIANC_FXLMSFILTER_H

#include "LMSFilter.h"
#include "FIRFilter.h"

template<int fx_filter_length, int filter_length>
class FxLMSFilter : public LMSFilter<filter_length> {

public:
    typedef std::array<float, fx_filter_length> fx_filter_coeffs_array;

    FxLMSFilter(float alpha_val, const fx_filter_coeffs_array s_filter) :
            LMSFilter<filter_length>(alpha_val), _s_filter{s_filter} {};

    sample_type lms_step(sample_type x_reference_sample, sample_type error_sample) {
        sample_type filtered_x_sample = _s_filter.fir_step(x_reference_sample);
        return LMSFilter<filter_length>::lms_step(filtered_x_sample, error_sample);
    }

private:
    FIRFilter<fx_filter_length> _s_filter;

};


#endif //RPIANC_FXLMSFILTER_H
