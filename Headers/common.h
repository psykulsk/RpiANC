//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_COMMON_H
#define RPIANC_COMMON_H

#include <array>
#include <limits>

typedef double sample_type;
typedef int32_t fixed_sample_type;
const long unsigned int FILTER_LENGTH = 21;
const long unsigned int FX_FILTER_LENGTH = 40;

sample_type inline signed_fixed_to_floating(fixed_sample_type sample) {
    sample_type max = std::numeric_limits<fixed_sample_type>::max();
    return static_cast<sample_type>(sample)/max;
}

fixed_sample_type inline floating_to_signed_fixed(sample_type sample) {
    sample_type max = std::numeric_limits<fixed_sample_type>::max();
    return static_cast<fixed_sample_type>(max*sample);
}
#endif //RPIANC_COMMON_H
