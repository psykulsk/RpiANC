//
// Created by pitersk on 24.04.19.
//

#ifndef RPIANC_SUBBAND_FILTERING_CONSTANTS_H
#define RPIANC_SUBBAND_FILTERING_CONSTANTS_H

#include "common.h"

const size_t ANALYSIS_FILTER_LENGTH = 14;
const size_t SYNTHESIS_FILTER_LENGTH = 14;
const std::array<sample_type, ANALYSIS_FILTER_LENGTH> ANALYSIS_COEFFS_1 = {
        -0.003371,
        -0.082363,
        -0.039126,
        0.042823,
        0.145794,
        0.247086,
        0.263891,
        0.247696,
        0.162186,
        0.011043,
        -0.004178,
        -0.003573,
        0.007366,
        -0.000042
};
const std::array<sample_type, ANALYSIS_FILTER_LENGTH> ANALYSIS_COEFFS_2 = {
        0.007720,
        0.188728,
        0.137579,
        -0.159355,
        -0.323804,
        -0.080481,
        0.221358,
        0.155586,
        -0.012457,
        -0.007179,
        0.001624,
        0.001440,
        -0.004558,
        0.000028
};
const std::array<sample_type, ANALYSIS_FILTER_LENGTH> ANALYSIS_COEFFS_3 = {
        -0.001272,
        -0.031101,
        -0.166393,
        0.210611,
        0.069029,
        -0.316473,
        0.164994,
        0.145237,
        -0.193137,
        0.003165,
        -0.000751,
        -0.000639,
        0.001247,
        -0.000006
};
const std::array<sample_type, ANALYSIS_FILTER_LENGTH> ANALYSIS_COEFFS_4 = {
        0.001024,
        0.025045,
        0.020942,
        -0.024601,
        -0.036443,
        0.162398,
        -0.235637,
        0.271863,
        -0.251531,
        0.141506,
        -0.039539,
        -0.034346,
        0.087094,
        -0.000509
};

const std::array<sample_type, SYNTHESIS_FILTER_LENGTH> SYNTHESIS_COEFFS_1 = {
        -0.001605,
        0.039253,
        -0.022094,
        -0.020376,
        0.039659,
        0.567149,
        0.961653,
        1.096187,
        1.024223,
        0.569257,
        0.153376,
        -0.180484,
        -0.363643,
        -0.002117
};
const std::array<sample_type, SYNTHESIS_FILTER_LENGTH> SYNTHESIS_COEFFS_2 = {
        0.001057,
        -0.025858,
        0.014379,
        0.013202,
        -0.038105,
        -0.080482,
        0.558480,
        0.865739,
        -0.295552,
        -1.157155,
        -0.604855,
        0.615688,
        0.744089,
        0.004347
};
const std::array<sample_type, SYNTHESIS_FILTER_LENGTH> SYNTHESIS_COEFFS_3 = {
        -0.000494,
        0.012035,
        -0.006755,
        -0.006220,
        0.050566,
        -0.765081,
        0.624300,
        0.568258,
        -1.175014,
        0.255224,
        0.780458,
        -0.654500,
        -0.156387,
        -0.000918
};
const std::array<sample_type, SYNTHESIS_FILTER_LENGTH> SYNTHESIS_COEFFS_4 = {
        -0.013861,
        0.338855,
        -0.189422,
        -0.174228,
        0.583197,
        -0.993952,
        1.136338,
        -0.918645,
        0.559310,
        -0.108629,
        -0.055820,
        0.056832,
        0.071948,
        0.000419
};
#endif //RPIANC_SUBBAND_FILTERING_CONSTANTS_H
