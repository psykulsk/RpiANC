//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_COMMON_H
#define RPIANC_COMMON_H

#include <array>
#include <limits>
#include <vector>
#include <fstream>
#include <assert.h>

typedef float sample_type;
typedef int32_t fixed_sample_type;
const long unsigned int FILTER_LENGTH = 30;
const long unsigned int FX_FILTER_LENGTH = 5;
const sample_type scale = std::numeric_limits<fixed_sample_type>::max();

sample_type inline signed_fixed_to_floating(fixed_sample_type sample) {
    return static_cast<sample_type>(sample)/scale;
}

fixed_sample_type inline floating_to_signed_fixed(sample_type sample) {
    return static_cast<fixed_sample_type>(scale*sample);
}

void save_vector_to_file(const std::string &filename, const std::vector<fixed_sample_type> &signal);

#endif //RPIANC_COMMON_H
