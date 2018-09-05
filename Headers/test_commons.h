//
// Created by pitersk on 03.09.18.
//

#ifndef RPIANC_TEST_COMMONS_H
#define RPIANC_TEST_COMMONS_H


#define USE_MATH_DEFINES

#include "math.h"
#include <vector>
#include "../Headers/matplotlibcpp.h"
#include "../Headers/FIRFilter.h"
#include "../Headers/LMSFilter.h"
#include "../Headers/common.h"

using std::vector;

typedef vector<sample_type> signal_vec;

namespace plt = matplotlibcpp;

signal_vec singen(long int nr_of_samples, double sampling_freq, double amplitude, double freq, double phase_shift) {
    signal_vec sine_sig;
    for (int i = 0; i < nr_of_samples; ++i) {
        double curr_phase = phase_shift + 2.0 * M_PI * freq * (double) i / sampling_freq;
        sine_sig.push_back(amplitude * sin(curr_phase));
    }
    return sine_sig;
}

signal_vec gen_func() {
    long int number_of_samples = 5000;
    double sampling_freq = 20000.0;
    signal_vec x1 = singen(number_of_samples, sampling_freq, 0.3, 4000.0, 0.0);
    signal_vec x2 = singen(number_of_samples, sampling_freq, 1.0, 10.0, 0.0);
    signal_vec sum = x1;
    std::transform(x1.begin(), x1.end(), x2.begin(), sum.begin(), std::plus<double>());
    return sum;

}
#endif //RPIANC_TEST_COMMONS_H
