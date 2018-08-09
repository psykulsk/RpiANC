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

signal_vec singen(int len, double amplitude, double freq, double phase_shift){
    signal_vec sine_sig;
    for(int i=0; i<len; ++i){
        double curr_phase = phase_shift+2*M_PI*i/freq;
        sine_sig.push_back(amplitude*sin(curr_phase));
    }
    return sine_sig;
}

signal_vec fir_test(signal_vec input){
    filter_coeffs_array
    FIRFilter filter = FIRFilter()
}


int main(){
   signal_vec x = singen(10000, 1.0, 1000.0, 0.0);

   plt::plot(x);
   plt::show();
}
