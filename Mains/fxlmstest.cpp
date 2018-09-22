//
// Created by pitersk on 21.08.18.
//

#include "../Headers/FxLMSFilter.h"
#include "../Headers/test_commons.h"

void fxlmstest_fixed() {
    FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array s_filter_coeffs = {
            0.0409337972357249, 0.024043402026782, 0.0119930412581248,
            0.00483106060833201, 0.00144698955374871,
            9.30221684750573e-05};
    FIRFilter<FX_FILTER_LENGTH> s_filter(s_filter_coeffs);
    s_filter_coeffs = {
            0.2409337972357249, 0.094043402026782, 0.0119930412581248,
            0.00483106060833201, 0.00144698955374871,
            9.30221684750573e-05};
    FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(5.0, s_filter_coeffs);
    unsigned long number_of_samples = 5000;
    double sampling_freq = 20000.0;
    signal_vec noise_signal = singen(number_of_samples, sampling_freq, 0.8, 1000.0, 0.0);
    signal_vec original_signal = singen(number_of_samples, sampling_freq, 1.0, 100.0, 0.0);
    signal_vec reference_signal = original_signal;
    std::transform(original_signal.begin(), original_signal.end(), noise_signal.begin(), reference_signal.begin(),
                   std::plus<>());
    fixed_sample_type error_sample = 0;
    signal_vec output;
    signal_vec correction_vect;
    signal_vec error_signal;
    for (unsigned long i = 0; i < number_of_samples; ++i) {
        sample_type correction_sample = fxlms_filter.lms_step(reference_signal.at(i),
                                                              signed_fixed_to_floating(error_sample));
        sample_type filtered_correction_sample = s_filter.fir_step(correction_sample);
        sample_type corrected_sample =
                reference_signal.at(i) - filtered_correction_sample;
        error_sample = floating_to_signed_fixed(original_signal.at(i)) - floating_to_signed_fixed(corrected_sample);
        error_signal.push_back(abs(error_sample));
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

void fxlmstest() {
    FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH>::fx_filter_coeffs_array s_filter_coeffs = {
            0.0409337972357249, 0.024043402026782, 0.0119930412581248,
            0.00483106060833201, 0.00144698955374871,
            9.30221684750573e-05};
    FIRFilter<FX_FILTER_LENGTH> s_filter(s_filter_coeffs);
    FxLMSFilter<FX_FILTER_LENGTH, FILTER_LENGTH> fxlms_filter(5.0, s_filter_coeffs);
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
    fxlmstest_fixed();
    return 0;
}