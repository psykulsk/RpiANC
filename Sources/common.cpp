//
// Created by pitersk on 09.10.18.
//

#include "../Headers/common.h"


void save_vector_to_file(const std::string &filename, const std::vector<fixed_sample_type> &signal){
    std::ofstream file;
    file.open(filename, std::ios::binary);

    assert(file.is_open());
    for (fixed_sample_type s : signal)
        file.write((char *) &s, sizeof(fixed_sample_type));
    file.close();
}

void save_vector_to_file(const std::string &filename, const std::vector<long> &signal){
    std::ofstream file;
    file.open(filename, std::ios::binary);

    assert(file.is_open());
    for (fixed_sample_type s : signal)
        file.write((char *) &s, sizeof(long));
    file.close();
}
