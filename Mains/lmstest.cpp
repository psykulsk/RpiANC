//
// Created by pitersk on 09.08.18.
//

#define USE_MATH_DEFINES

#include "math.h"
#include <vector>
#include "../Headers/matplotlibcpp.h"
#include "../Headers/FIRFilter.h"

using std::vector;

typedef vector<double> signal_vec;

namespace plt = matplotlibcpp;

signal_vec singen(int nr_of_samples, double sampling_freq, double amplitude, double freq, double phase_shift) {
    signal_vec sine_sig;
    for (int i = 0; i < nr_of_samples; ++i) {
        double curr_phase = phase_shift + 2.0 * M_PI * freq * (double) i / sampling_freq;
        sine_sig.push_back(amplitude * sin(curr_phase));
    }
    return sine_sig;
}

signal_vec gen_func(){
    signal_vec x1 = singen(2000, 20000.0, 0.3, 4000.0, 0.0);
    signal_vec x2 = singen(2000, 20000.0, 1.0, 10.0, 0.0);
    signal_vec sum = x1;
    std::transform(x1.begin(), x1.end(), x2.begin(), sum.begin(), std::plus<double>());
    return sum;

}

void fir_test() {
    signal_vec input = gen_func();
    // fir values from octave function fir1(20, 0.1)
    filter_coeffs_array fir = {9.30221684750573e-05,0.00144698955374871,0.00483106060833201,0.0119930412581248,0.024043402026782,0.0409337972357249,0.0612310733616234,0.0822835022069283,0.100759234867788,0.113421421451563,0.117926910521821,0.113421421451563,0.100759234867788,0.0822835022069283,0.0612310733616234,0.0409337972357249,0.024043402026782,0.0119930412581248,0.00483106060833201,0.00144698955374871,9.30221684750573e-05};
    FIRFilter filter = FIRFilter(fir);
    signal_vec output;
    for (double sample: input) {
        output.push_back(filter.fir_step(sample));
    }

    plt::subplot(1,2,1);
    plt::plot(input);
    plt::subplot(1,2,2);
    plt::plot(output);
    plt::show();

}


int main() {
    fir_test();
}
