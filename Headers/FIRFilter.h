//
// Created by pitersk on 29.07.18.
//

#ifndef RPIANC_FIRFILTER_H
#define RPIANC_FIRFILTER_H

#include "common.h"

class FIRFilter {
public:
    FIRFilter(filter_array coefficients): _filter_coefficients{coefficients}{

    }

    sample_type fir_step(sample_type new_sample);

    void set_coefficients(filter_array new_coefficients){
        _filter_coefficients = new_coefficients;
    }
    void reset_sample_buffer(){
        _samples_buffer = {0};
    }
private:
    filter_array _filter_coefficients = {0};
    filter_array _samples_buffer = {0};
};


#endif //RPIANC_FIRFILTER_H
