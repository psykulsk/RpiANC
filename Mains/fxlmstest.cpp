//
// Created by pitersk on 21.08.18.
//

#include "../Headers/FxLMSFilter.h"
#include "../Headers/test_commons.h"

void fxlmstest() {
    FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array s_filter_coeffs = {
            1.00};
    FIRFilter<FX_FILTER_LENGTH> s_filter(s_filter_coeffs);
    FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(0.0005, s_filter_coeffs);
    unsigned long number_of_samples = 5000;
    double sampling_freq = 20000.0;
    signal_vec noise_signal = singen(number_of_samples, sampling_freq, 0.8, 1000.0, 0.0);
    signal_vec original_signal = singen(number_of_samples, sampling_freq, 1.0, 100.0, 0.0);
    signal_vec reference_signal = original_signal;
    std::transform(original_signal.begin(), original_signal.end(), noise_signal.begin(), reference_signal.begin(),
                   std::plus<>());
    sample_type error_sample = 0.0;
    signal_vec output;
    signal_vec correction_vect;
    signal_vec error_signal;
    for (unsigned long i = 0; i < number_of_samples; ++i) {
        sample_type correction_sample = fxlms_filter.lms_step(reference_signal.at(i), error_sample);
        sample_type filtered_correction_sample = s_filter.fir_step(correction_sample);
        sample_type corrected_sample = reference_signal.at(i) - filtered_correction_sample;
        error_sample = original_signal.at(i) - corrected_sample;
        error_signal.push_back(error_sample);
        correction_vect.push_back(correction_sample);
        output.push_back(corrected_sample);
    }
    signal_vec x(number_of_samples);
    std::iota(x.begin(), x.end(), 0);
    plt::subplot(3, 1, 1);
    plt::plot(reference_signal);
    plt::subplot(3, 1, 2);
    plt::plot(output);
    plt::subplot(3, 1, 3);
    plt::semilogy(x, error_signal);
    plt::show();
}


int main() {
    fxlmstest();
    return 0;
}