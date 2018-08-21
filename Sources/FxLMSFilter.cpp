//
// Created by pitersk on 21.08.18.
//

#include "../Headers/FxLMSFilter.h"

sample_type FxLMSFilter::lms_step(sample_type x_reference_sample, sample_type error_sample) {
    sample_type filtered_x_sample = _s_filter.fir_step(x_reference_sample);
    return LMSFilter::lms_step(filtered_x_sample, error_sample);
}
