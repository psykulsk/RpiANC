//
// Created by pitersk on 29.07.18.
//

#ifndef RPIANC_FIRFILTER_H
#define RPIANC_FIRFILTER_H

#include "common.h"

class FIRFilter {
public:
    FIRFilter(): _filter_coefficients{{0.0f}}, _samples_buffer{{0}}{}
    FIRFilter(filter_coeffs_array coefficients): _filter_coefficients{coefficients}, _samples_buffer{0}{

    }

    sample_type fir_step(sample_type new_sample);

    filter_coeffs_array get_coefficients();

    void set_coefficients(filter_coeffs_array new_coefficients){
        _filter_coefficients = new_coefficients;
    }
    void reset_sample_buffer(){
        _samples_buffer = {0};
    }
    void lms_coefficients_update(double update_step);
private:
    filter_coeffs_array _filter_coefficients;
    samples_array _samples_buffer;
};


#endif //RPIANC_FIRFILTER_H
