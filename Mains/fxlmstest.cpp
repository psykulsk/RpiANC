//
// Created by pitersk on 21.08.18.
//

#include <fstream>
#include <iterator>
#include "../Headers/FxLMSFilter.h"
#include "../Headers/test_commons.h"
#include "../Headers/common.h"

void fxlmstest_fixed() {
    FxLMSFilter<FX_FILTER_LENGTH_TEST, FILTER_LENGTH_TEST>::fx_filter_coeffs_array s_filter_coeffs = {
            0.0409337972357249, 0.024043402026782, 0.0119930412581248,
            0.00483106060833201, 0.00144698955374871,
            9.30221684750573e-05};
    FIRFilter<FX_FILTER_LENGTH_TEST> s_filter(s_filter_coeffs);
    s_filter_coeffs = {
            0.2409337972357249, 0.094043402026782, 0.0119930412581248,
            0.00483106060833201, 0.00144698955374871,
            9.30221684750573e-05};
    FxLMSFilter<FX_FILTER_LENGTH_TEST, FILTER_LENGTH_TEST> fxlms_filter(1.0, s_filter_coeffs);
    unsigned long number_of_samples = 5000;
    double sampling_freq = 20000.0;
    signal_vec noise_signal = singen(number_of_samples, sampling_freq, 0.8, 1000.0, 0.0);
    signal_vec original_signal = singen(number_of_samples, sampling_freq, 1.0, 100.0, 0.0);
    signal_vec reference_signal = original_signal;
    std::transform(original_signal.begin(), original_signal.end(), noise_signal.begin(),
                   reference_signal.begin(),
                   std::plus<>());
    fixed_sample_type error_sample = 0;
    signal_vec output;
    signal_vec correction_vect;
    signal_vec error_signal;
    for (unsigned long i = 0; i < number_of_samples; ++i) {
        sample_type correction_sample = fxlms_filter.lms_step(reference_signal.at(i),
                                                              signed_fixed_to_floating(
                                                                      error_sample));
        sample_type filtered_correction_sample = s_filter.fir_step(correction_sample);
        sample_type corrected_sample =
                reference_signal.at(i) - filtered_correction_sample;
        error_sample = floating_to_signed_fixed(original_signal.at(i)) -
                       floating_to_signed_fixed(corrected_sample);
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

void ReadFromFile(std::vector<fixed_sample_type> &x, const std::string &file_name) {
    std::ifstream read_file;
    read_file.open(file_name, std::ios::binary);
    assert(read_file.is_open());

//    std::copy(std::istream_iterator<fixed_sample_type>(read_file), std::istream_iterator<fixed_sample_type>(),
//              std::back_inserter(x));
    while (!read_file.eof()) {
        fixed_sample_type sample;
        read_file.read((char *) &sample, sizeof(fixed_sample_type));
        x.push_back(sample);
    }
    read_file.close();
}

void fxlmstest_recorded_data() {
    std::vector<fixed_sample_type> ref_vec, err_vec;
    ReadFromFile(ref_vec, "../test_rec/ref_mic_test.dat");
    ReadFromFile(err_vec, "../test_rec/err_mic_test.dat");
    FxLMSFilter<FX_FILTER_LENGTH_TEST, FILTER_LENGTH>::fx_filter_coeffs_array s_filter_coeffs = {
            1.0
    };
    FxLMSFilter<FX_FILTER_LENGTH_TEST, FILTER_LENGTH> fxlms_filter(1.0, s_filter_coeffs);
    sample_type error_sample = 0;
    signal_vec output;
    signal_vec correction_vect;
    signal_vec error_signal;
    signal_vec error_signal_abs;
    signal_vec output_signal;
    signal_vec original_error;
    signal_vec original_error_abs;
    fixed_signal_vec output_signal_fixed;
    for (unsigned long i = 0; i < ref_vec.size(); ++i) {
        sample_type correction_sample = fxlms_filter.lms_step(
                signed_fixed_to_floating(ref_vec.at(i)),
                error_sample);
        sample_type filtered_correction_sample = correction_sample;
        sample_type err_sample = signed_fixed_to_floating(err_vec.at(i));

        error_sample = err_sample + filtered_correction_sample;
        error_signal.push_back(error_sample);
        error_signal_abs.push_back(std::abs(error_sample));
        output_signal.push_back(error_sample);
        output_signal_fixed.push_back(floating_to_signed_fixed(error_sample));
        correction_vect.push_back(filtered_correction_sample);
        original_error.push_back(err_sample);
        original_error_abs.push_back(std::abs(err_sample));
    }
    signal_vec x(ref_vec.size());

    unsigned int number_of_plots = 6;
    std::iota(x.begin(), x.end(), 0);
    plt::subplot(number_of_plots, 1, 1);
    plt::plot(ref_vec);
    plt::title("Reference mic signal");
    plt::subplot(number_of_plots, 1, 2);
    plt::plot(correction_vect);
    plt::title("Correction signal");
    plt::subplot(number_of_plots, 1, 3);
    plt::plot(output_signal);
    plt::title("Corrected signal");
    plt::subplot(number_of_plots, 1, 4);
    plt::plot(original_error);
    plt::title("Original, uncorrected signal");
    plt::subplot(number_of_plots, 1, 5);
    plt::semilogy(x, original_error_abs, "b-");
    plt::title("Original signal power");
    plt::subplot(number_of_plots, 1, 6);
    plt::semilogy(x, error_signal_abs, "r-");

    plt::title("Corrected signal power");
    plt::show();
    save_vector_to_file("rec/output_signal.raw", output_signal_fixed);
}

void fxlmstest() {
    FxLMSFilter<FX_FILTER_LENGTH_TEST, FILTER_LENGTH_TEST>::fx_filter_coeffs_array s_filter_coeffs = {
            0.0409337972357249, 0.024043402026782, 0.0119930412581248,
            0.00483106060833201, 0.00144698955374871,
            9.30221684750573e-05};
    FIRFilter<FX_FILTER_LENGTH_TEST> s_filter(s_filter_coeffs);
    FxLMSFilter<FX_FILTER_LENGTH_TEST, FILTER_LENGTH_TEST> fxlms_filter(1.5, s_filter_coeffs);
    unsigned long number_of_samples = 5000;
    double sampling_freq = 20000.0;
    signal_vec noise_signal = singen(number_of_samples, sampling_freq, 0.8, 1000.0, 0.0);
    signal_vec original_signal = singen(number_of_samples, sampling_freq, 1.0, 100.0, 0.0);
    signal_vec reference_signal = original_signal;
    std::transform(original_signal.begin(), original_signal.end(), noise_signal.begin(),
                   reference_signal.begin(),
                   std::plus<>());
    sample_type error_sample = 0.0;
    signal_vec output;
    signal_vec error_signal;
    signal_vec correction_vect;
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
//    fxlmstest_recorded_data();
//    fxlmstest_fixed();
    fxlmstest();
    return 0;
}