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
const sample_type scale = std::numeric_limits<fixed_sample_type>::max();

sample_type inline signed_fixed_to_floating(fixed_sample_type sample) {
    return static_cast<sample_type>(sample)/scale;
}

fixed_sample_type inline floating_to_signed_fixed(sample_type sample) {
    if(sample > 1.0f)
        return std::numeric_limits<fixed_sample_type>::max();
    if(sample < -1.0f)
        return std::numeric_limits<fixed_sample_type>::min();
    return static_cast<fixed_sample_type>(scale*sample);
}

void save_vector_to_file(const std::string &filename, const std::vector<fixed_sample_type> &signal);

void save_vector_to_file(const std::string &filename, const std::vector<long> &signal);

//template<class InputIterator>
//void save_container_to_file(const std::string &filename, InputIterator first, InputIterator last){
//    std::ofstream file;
//    file.open(filename, std::ios::binary);
//
//    assert(file.is_open());
//    while(first != last){
//        file.write((char *) first, sizeof(fixed_sample_type));
//        ++first;
//    }
//    for (fixed_sample_type s : signal)
//        file.write((char *) &s, sizeof(fixed_sample_type));
//    file.close();
//}
#endif //RPIANC_COMMON_H
