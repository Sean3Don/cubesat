/* 
 * File:   Control.h
 * Author: Dmitriy
 *
 * This library contains the control system. It takes as inputs pointers to two
 * structs, inputs and outputs, reads the inputs, and writes the outputs. The runner
 * then passes the outputs to the setters
 *
 * Created on May 9, 2014, 4:30 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

struct inputs {
    float enc_angle;
    float AHRS_angle;
};


struct outputs {
    unsigned int pulse_width;
};



void init_control(void);
/* Function: init_control
 *
 * Whatever initialization is needed for the controller (if any)

 */

 int Control(struct inputs *in, struct outputs *out,float command);
/* Function: control
 *
 * here is where you implement the controller. The inputs structs has all the
 * sensor values, and you should write the values you want passed to the getters
 * in the outputs struct
 *
 *
 */


#endif	/* CONTROL_H */

