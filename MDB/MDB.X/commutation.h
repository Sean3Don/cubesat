/* 
 * File:   commutation.h
 * Author: Dmitriy
 *
 * Created on July 22, 2014, 9:38 AM
 */

#include <stdint.h>

//#define TEST_COMMAND //test to make sure you can send commands over SPI and that the safety check is working

#define TEST_SPEED_READINGS

//#define test_clock_period

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




#endif	/* COMMUTATION_H */

