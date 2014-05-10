/*
 * File:   Control.c
 * Author: Dmitriy
 *
 * This library contains the control system. It takes as inputs pointers to two
 * structs, inputs and outputs, reads the inputs, and writes the outputs. The runner
 * then passes the outputs to the setters
 *
 * Created on May 9, 2014, 4:30 PM
 */

#include "Control.h"


void init_control(void){

}

 void Control(struct inputs *in, struct outputs *out){
    int angle = in->enc_angle;

    out->pulse_width = (((unsigned)angle)*1000/16383) + 1000;
}