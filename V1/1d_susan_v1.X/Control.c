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
#include "stdint.h"

#define MAX_ENC_READING 3300
#define PI 3.14159

#define MOTOR_VOLTAGE_LIMIT 6

//#define PID_CONTROL
//#define BANG_BANG
#define SMART_BANG


//constants for control
#define Ki -.01
#define Kp -3
#define Kd .5
#define ON 1
#define OFF 0



float integ = 0;
float ret_val = 0;
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
        integ = integ + error;
        return integ;
    } else {
        return 0;
    }
}

//computes the derivative of the error, (doesn't take time step into account)
float prev = 0;

float derivative(float error) {
    float derv = error - prev;
    prev = error;
    return derv;
}

//clears integrator (resets to zero)

void integral_clear() {
    integ = 0;
    ret_val = 0;
}

//turns intergator off (integrator will clear and return zero)

void integral_on_off(int on) {
    if (!on) {
        integral_clear();
        inton = OFF;
    } else {
        inton = ON;
    }
}

/*This function spits out an error which determines which way the
 satellite should spin. It computes the distance to the commanded
 point and makes it negative if clockwise rotation is needed ( i.e. if travelling from
 currAng to command would require a clockwise rotation of less than pi)  */

float compute_error(float command, float currAng) {
    float cw;
    float ccw;

    if (currAng > command) {
        cw = currAng - command;
        ccw = 2 * PI - currAng + command;
    } else {
        cw = currAng + 2 * PI - command;
        ccw = command - currAng;
    }
    if (cw < ccw) {
        return -cw;
    } else {
        return ccw;
    }
}

#ifdef PID_CONTROL

void init_control(void) {
    inton = 1;
}
#endif

// given a desired voltage and a battery voltage (both in volts), this function
// computes the duty cycle necessary to get the desired voltage. Duty cycle is between
// -1000 and 1000 (negative for negative voltages).

int16_t convert_to_duty_cycle(float voltage, float batVoltage) {
    int16_t mCom = (int16_t) (1000 * voltage / batVoltage);
    return mCom;
}
#ifdef PID_CONTROL

int Control(struct inputs *in, struct outputs *out, float command) {

    float angle = in->AHRS_angle;


    float error = compute_error(command, angle);
    //clear integrator on 0 crossing to avoid craziness
    //    if( fabs(angle)<.01){
    //        integral_clear();
    //    }
    float drv = in->AHRS_yaw_rate;
    printf("angle %f\n", angle);
    float cntrlVoltage = Kp * proportional(error) + Ki * integral(error) + Kd*drv; // this is in volts, and is converted to a duty cycle
    // printf("control voltage %f\n", cntrlVoltage);
    //    printf("prop %f\n",proportional(error));
    //    printf("int %f\n", integral(error));
    //    printf("drv %f\n",drv);
    if (cntrlVoltage > MOTOR_VOLTAGE_LIMIT) {
        cntrlVoltage = MOTOR_VOLTAGE_LIMIT;
    }
    if (cntrlVoltage < -MOTOR_VOLTAGE_LIMIT) {
        cntrlVoltage = -MOTOR_VOLTAGE_LIMIT;
    }

    out->motor_command = convert_to_duty_cycle(cntrlVoltage, in->bat_voltage);

    printf(" error %f \n", error);
    if (fabs(drv) == 0 && fabs(error) == .05) { // if stabilized
        return 1;
    } else {
        return 0;
    }
}
#endif



// duty cycles
#define BANGHIGH 400
#define BANGLOW -400



// this function just computes the difference between the two last measured points
// it's called kspeed because it does not account for sampling rate. Therefore, it
// is like the real speed in radians per second multiplied by some constant.
float last_angle = 0;

//float compute_kspeed(float curr_angle) {
//    float kspeed = compute_error(curr_angle, last_angle); // compute distance between last two angles measured;
//    last_angle = curr_angle;
//    return kspeed;
//}

// looks at the error and the speed and computes which of the two available duty cycles
// it should command. Eventually it should stabilize and flip between the two at a
// duty cycle which keeps the thing vaguely in the same place.

//int16_t compute_output(float error, float kspeed) {
//
//}
#ifdef BANG_BANG

int Control(struct inputs *in, struct outputs *out, float command) {
    float angle = in->AHRS_angle;

    float error = compute_error(command, angle);
    if (error > 0) {
        out->motor_command = BANGLOW;
    } else {
        out->motor_command = BANGHIGH;
    }
    printf("m com %d", out->motor_command);
    printf(" error %f \n", error);
    if (fabs(error) == 0) { // if stabilized
        return 1;
    } else {
        return 0;
    }
}
#endif

#ifdef SMART_BANG

#define SPEED_THRESH .05
#define SPEED_THRESH2 .1

typedef enum {
    start,
    wait,
    stop
} state_type_t;

state_type_t state;

void init_control(void) {
    state = start;
}

//returns 1 if both have the same sign, otherwiser returns 0

int same_sign(float num1, float num2) {
    if (num1 >= 0 && num2 >= 0) {
        return 1;
    }
    if (num1 <= 0 && num2 <= 0) {
        return 1;
    }
    return 0;
}

int Control(struct inputs *in, struct outputs *out, float command) {
    static float errAtStart;
    float angle = in->AHRS_angle;

    float error = compute_error(command, angle);
    float speed = in->AHRS_yaw_rate;
    printf("speed) %f\n", speed);
    printf(" error %f \n", error);

    if (!same_sign(error, speed)) { // if your traveling in such as way as to increase error, be reversing
        if (error > 0) {
            out->motor_command = BANGLOW;
        } else {
            out->motor_command = BANGHIGH;
        }
        state = start;
    } else {

        switch (state) {
            case start:
                if (fabs(error) < 0) { // if error is small enough do nothing
                } else {
                    if (error > 0) {
                        out->motor_command = BANGLOW;
                    } else {
                        out->motor_command = BANGHIGH;
                    }
                    state = wait;
                    errAtStart = error;
                }
                break;

            case wait: // wait till you get half way, then start slowing down
                printf("erratstart %f\n", errAtStart);
                if (fabs(error) < (fabs(errAtStart) / 2)) { // if error and speed have different signs it's bad
                    if (errAtStart > 0) {
                        out->motor_command = BANGHIGH;
                    } else {
                        out->motor_command = BANGLOW;
                    }
                    state = stop;
                }
                break;

            case stop:
                if (fabs(speed) < SPEED_THRESH) {
                    state = start;
                }
                break;

            default:
                break;
        }
    }
    printf("state %d \n\n", state);


    //    if (fabs(drv) == 0 && fabs(error) == .05) { // if stabilized
    //        return 1;
    //    } else {
    //        return 0;
    //    }
    return 0;
}







#endif





