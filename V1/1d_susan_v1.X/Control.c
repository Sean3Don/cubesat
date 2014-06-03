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
#include "xc.h"
#include "Control.h"
#include "serial.h"
#include "board.h"

#define MAX_ENC_READING 3300
#define PI 3.14159

void init_control(void){

}

 void Control(struct inputs *in, struct outputs *out){
 
    float angle = in->AHRS_angle;
    printf("%f \n", angle);

    float con=(angle/PI*500)+1000;
    out->pulse_width =(unsigned int)con;
}