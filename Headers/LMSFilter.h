//
// Created by pitersk on 29.07.18.
//

#ifndef RPIANC_LMSFILTER_H
#define RPIANC_LMSFILTER_H


#include "common.h"
#include "FIRFilter.h"


template<int filter_length>
class LMSFilter {
public:

    typedef std::array<sample_type, filter_length> samples_array;
    typedef std::array<float, filter_length> filter_coeffs_array;
    FIRFilter<filter_length> fir_filter;

    LMSFilter(float alpha_val) : _alpha{alpha_val}, _lms_coefficients{{0}}, _samples_buffer{{0}} {
        fir_filter.set_coefficients(_lms_coefficients);
    }

    LMSFilter(float alpha_val, filter_coeffs_array initial_filter) : _alpha{alpha_val},
                                                                     _lms_coefficients{
                                                                             0.0},
                                                                     _samples_buffer{{0.0}} {
        fir_filter.set_coefficients(initial_filter);
    }

    virtual sample_type lms_step(sample_type x_reference_sample, sample_type error_sample,
                                 sample_type unfiltered_x_sample) {
        // Shift samples buffer
        for (long int i = filter_length - 1; i >= 1; --i) {
            _samples_buffer[i] = _samples_buffer[i - 1];
        }
        _samples_buffer[0] = x_reference_sample;
        // Update filter coefficients
        lms_filter_update(
                -(_alpha) *
                static_cast<float>(error_sample));
        // Perform filtering step, to generate new y correction sample
        return fir_filter.fir_step(unfiltered_x_sample);
    }

    void lms_filter_update(float update_step) {
        filter_coeffs_array filter_coeffs = fir_filter.get_coefficients();
        for (int i = 0; i < filter_length; ++i) {
            filter_coeffs.at(i) =  _lms_leak_factor*filter_coeffs.at(i)
                                  + _samples_buffer.at(i) * update_step;
        }
        fir_filter.set_coefficients(filter_coeffs);
    }

private:
    float _alpha;
    float _lms_leak_factor = 0.99999f;
    filter_coeffs_array _lms_coefficients;
    samples_array _samples_buffer;
};


#endif //RPIANC_LMSFILTER_H
