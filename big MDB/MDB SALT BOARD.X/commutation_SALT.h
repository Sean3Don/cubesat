/* 
 * File:   commutation_SALT.h
 * Author: Dmitriy
 *
 * Created on August 7, 2014, 1:47 PM
 */

#ifndef COMMUTATION_SALT_H
#define	COMMUTATION_SALT_H


#include <stdint.h>


//define this if you want the MDB to communicate with the UNO over SPI
//#define SPI_COMS

//define this if you want to command the MDB with PWM from the UNO (PWMCOMMAND must be defined)
#define PWM_COMS

// define this if you want to send PWM commands to the MDB
#define PWMCOMMAND

//define this if you want to send torque commands to the MDB and have it control the torque
//#define TORQUE_COMMAND


//#define TEST_COMMAND //test to make sure you can send commands over SPI and that the safety check is working
//#define test_clock_period
#define TEST_SPEED_READINGS

//#define TEST_TORQUE_COMMAND

// the following three #defines only do something if you are in torque control mode
#define MAX_SPEED 3500 // maximum allowable motor speed in RPM when using torque control. Does not affect motor speed in PWM control mode.
#define MAX_TORQUE 1500 // maximum allowable torque in micro Newton-Meters.
#define DUTY_CYCLE_MAX_SPEED 500 // the duty cycle at which the no load speed is the same as the max speed you just set (determined experimentally)

#ifndef COMMUTATION_H
#define	COMMUTATION_H


//Initializes timers for commutation
void commutationInit (void);



//Sets up the pins and interrups for the hall effect sensors. These pins will be
//polled and this will allow us to determine when to switch
void hallsInit (void);


//sets up pwm modules
void pwmInit(void);


//returns a velocity in RPM, negative means reverse
int16_t get_velocity(void);

//returns the PWM command the board is currently executing
int16_t get_current_command(void);

//sets a new duty cycle and direction for the commutator. Also does a safety check.

// uint16_t direction: A number between -1000 and 1000 which determines duty cycle
// and direction. Negative means reverse. If the number sent is outside of these
// bounds it is ignored.
void update_duty_cycle_and_direction(int16_t command);


// sets a new torque which the motor will produce until it hits maximum speed
// The motor currently in use is the faulhaber 2610 012B which can produce about
// 3 mNm all the way from stall to 7000 rpm. Therefore, tcommand should range
// from -3000 to +3000, and is measured in micro Newton-meters. Values outside
// of this range will be ignored. The maximum speed of this motor is 7000 rpm.
// Once the motor reaches that speed, no more torque will be produced. However,
// the max speed is set in a #define at the top of this file and can be adjusted.
void update_torque_command(int32_t  tcommand);




#endif	/* COMMUTATION_H */







#endif	/* COMMUTATION_SALT_H */

