//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_COMMON_H
#define RPIANC_COMMON_H

#include <array>

typedef double sample_type;
const long unsigned int FILTER_LENGTH = 21;
typedef std::array<sample_type, FILTER_LENGTH> samples_array;
typedef std::array<double, FILTER_LENGTH> filter_coeffs_array;

#endif //RPIANC_COMMON_H
