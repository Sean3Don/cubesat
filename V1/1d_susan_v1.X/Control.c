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
#include "math.h"

#define MAX_ENC_READING 3300
#define PI 3.14159


//constants for control
#define Ki 10
#define Kp 300
#define Kd 50
#define ON 1
#define OFF 0



float integ = 0;
float ret_val=0;
int inton = 0;

//calculates proportional component of control effort

float proportional(float error) {
    return error;
}

//calculates integral component of control effort


float integral(float error) {

    if (inton) {
//         integ = integ + angle;
//         //because of the friction, smooth increases in the integrator don't really work
//         // the following few lines make it more jumpy, and increase or decrease in jumps of 100
//         if(fabsf(ret_val-integ)>100*Ki){
//             ret_val=integ;
//         }
//         return ret_val;
        integ=integ+error;
        return integ;
    } else {
        return 0;
    }
}

//computes the derivative of the error, (doesn't take time step into account)
float prev=0;
float derivative (float error){
    float derv = error - prev;
    prev = error;
    return derv;
}

//clears integrator (resets to zero)

void integral_clear() {
    integ=0;
    ret_val=0;
}

//turns intergator off (integrator will clear and return zero)

void integral_on_off(int on) {
    if (!on) {
        integral_clear();
        inton = OFF;
    }else{
        inton=ON;
    }
}

/*This function spits out an error which determines which way the
 satellite should spin. It computes the distance to the commanded
 point and makes it negative if clockwise rotation is needed ( i.e. if travelling from
 currAng to command would require a clockwise rotation of less than pi)  */

float compute_error(float command, float currAng){
    float cw;
    float ccw;
    
    if (currAng > command){
        cw= currAng - command;
        ccw= 2*PI - currAng + command;
    }else{
        cw= currAng + 2*PI- command;
        ccw = command-currAng;
    }
    if(cw<ccw){
        return -cw;
    }else{
        return ccw;
    } 
}


void init_control(void) {
    inton = 1;
}

int Control(struct inputs *in, struct outputs *out, float command) {

    float angle = in->AHRS_angle;
    printf("%f \n", angle);

    float error = compute_error(command, angle);
    //clear integrator on 0 crossing to avoid craziness
//    if( fabs(angle)<.01){
//        integral_clear();
//    }
    float drv = derivative (error);

    float cntrl_eff = Kp*proportional(error)+ Ki*integral(error) + Kd*drv;
    
    unsigned int out_put;
    out_put = (unsigned int)(cntrl_eff+1500);

    out->pulse_width = out_put;

    printf("%f \n",error);
    if( fabs(drv)<.01 && fabs(error)<.1){ // if stabilized
        return 1;
    }else{
        return 0;
    }
}

