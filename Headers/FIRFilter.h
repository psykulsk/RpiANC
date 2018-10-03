//
// Created by pitersk on 29.07.18.
//

#ifndef RPIANC_FIRFILTER_H
#define RPIANC_FIRFILTER_H

#include "common.h"

template<int filter_length>
class FIRFilter {
public:
    typedef std::array<sample_type, filter_length> samples_array;
    typedef std::array<double, filter_length> filter_coeffs_array;

    FIRFilter() : _filter_coefficients{{0.0f}}, _samples_buffer{{0.0}} {}

    FIRFilter(filter_coeffs_array coefficients) : _filter_coefficients(coefficients), _samples_buffer{0.0} {

    }

    sample_type fir_step(sample_type new_sample) {
        sample_type new_val = 0;

        // Shift sample_buffer (FIFO style)
        for (long unsigned int i = filter_length - 1; i >= 1; --i) {
            _samples_buffer[i] = _samples_buffer[i - 1];
        }
        _samples_buffer[0] = new_sample;
        // Multiply and accumulate
        for (long unsigned int i = 0; i < filter_length; ++i) {
            new_val += _samples_buffer[i] * _filter_coefficients[i];
        }
        return new_val;
    }

    filter_coeffs_array get_coefficients() {
        return _filter_coefficients;
    }

    void set_coefficients(filter_coeffs_array new_coefficients) {
        _filter_coefficients = new_coefficients;
    }

    void reset_sample_buffer() {
        _samples_buffer = {0};
    }

private:
    filter_coeffs_array _filter_coefficients;
    samples_array _samples_buffer;
};


#endif //RPIANC_FIRFILTER_H
