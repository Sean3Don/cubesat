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


void init_control(void){

}

 void Control(struct inputs *in, struct outputs *out){
     static int i = 0;

     if(i<20){
         out->pulse_width=2000;
         i++;
     }
     if(i>=20){
         out->pulse_width=1000;
         i++;
     }
     if(i==40){
         i=0;
     }

//    int angle = in->enc_angle;
//    printf("%d \n", angle);
//    out->pulse_width = (((unsigned)angle)*1000/MAX_ENC_READING) + 1000;
}