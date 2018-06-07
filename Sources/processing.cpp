//
// Created by pitersk on 07.06.18.
//

#include "../Headers/processing.h"


void processing(sample_type *samples_buffer, long unsigned int buffer_length, Fir1fixed &fir_left, Fir1fixed &fir_right){

    for(int i=0; i<buffer_length; ++i){
        if(i%2){
            samples_buffer[i] = fir_left.filter(samples_buffer[i]);
        }else{

            samples_buffer[i] = fir_right.filter(samples_buffer[i]);
   //         *(samples_buffer+i) = 30000;
        }
    }

}