//
// Created by pitersk on 29.07.18.
//

#ifndef RPIANC_LMSFILTER_H
#define RPIANC_LMSFILTER_H


#include "common.h"

class LMSFilter {

public:
    LMSFilter(float alpha_val) : _alpha{alpha_val}{

    }



private:
    float _alpha;
    filter_array _lms_coefficients = {0};
};


#endif //RPIANC_LMSFILTER_H
