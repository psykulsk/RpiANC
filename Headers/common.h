//
// Created by pitersk on 07.06.18.
//

#ifndef RPIANC_COMMON_H
#define RPIANC_COMMON_H

#include <array>

typedef short int sample_type;
const long unsigned int FILTER_LENGTH = 20;
typedef std::array<sample_type, FILTER_LENGTH> filter_array;

#endif //RPIANC_COMMON_H
