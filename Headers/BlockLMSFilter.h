//
// Created by pitersk on 21.12.18.
//

#ifndef RPIANC_BLOCKLMSFILTER_H
#define RPIANC_BLOCKLMSFILTER_H

#include "common.h"
#include "constants.h"
#include "FIRFilter.h"

template<int filter_length>
class BlockLMSFilter {
public:

    typedef std::array<sample_type, filter_length> samples_array;
    typedef std::array<float, filter_length> filter_coeffs_array;
    typedef std::array<sample_type, filter_length> samples_block;

    FIRFilter<filter_length> fir_filter;

    BlockLMSFilter(float alpha_val) : _alpha{alpha_val}, _lms_coefficients{{0}} {
        fir_filter.set_coefficients(_lms_coefficients);
    }

    BlockLMSFilter(float alpha_val, filter_coeffs_array initial_filter) : _alpha{alpha_val},
                                                                          _lms_coefficients{0.0} {
        fir_filter.set_coefficients(initial_filter);
    }

    virtual void block_lms_step(const samples_block &reference_block,
            const samples_block &error_block, const samples_block &unfiltered_x_sample,
            samples_block &output) {
        block_lms_filter_update(reference_block, error_block);

        for(size_t i=0; i<filter_length; ++i){
            output.at(i) = fir_filter.fir_step(unfiltered_x_sample.at(i));
        }

    }

    void block_lms_filter_update(const samples_block &reference_block,
                                 const samples_block &error_block) {
        samples_block update_factors = {0.0f};

        for (size_t i = 0; i < filter_length; ++i) {
            for (size_t j = 0; j < filter_length; ++j) {
                update_factors.at(j) += reference_block.at(j) * error_block.at(i);
            }
        }

        filter_coeffs_array filter_coeffs = fir_filter.get_coefficients();
        for (int i = 0; i < filter_length; ++i) {
            filter_coeffs.at(i) = _lms_leak_factor * filter_coeffs.at(i)
                                  + update_factors.at(i) * _alpha;
        }
        fir_filter.set_coefficients(filter_coeffs);
    }

private:
    float _alpha;
    float _lms_leak_factor = 0.99999f;
    filter_coeffs_array _lms_coefficients;
};


#endif //RPIANC_BLOCKLMSFILTER_H
