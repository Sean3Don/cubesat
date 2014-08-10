/* 
 * File:   setters.h
 * Author: Dmitriy Rivkin
 *
 * This library contains functions necessary to set hardware states. For example,
 * this is where the functions that set the motor speeds would go. They are the interface
 * between the control system and the hardware. Initialization functions for output
 * hardware (ie things that physically affect the world, such as motors), go here too.
 * Code which pertains to obtaining values from sensors goes in the getters.h library.
 *
 *
 * Created on May 8, 2014, 2:27 PM
 */

#include "stdint.h"
//define this if you need to run the ESC calibration routine
//#define CALIBRATE_ESC

//define this if you want to play with the ESC
//#define ESC_TEST

//define this if you want to communicate with the MDB over SPI
//#define SPI_COMS_MDB

//define this if you want to be able to command motor speed with PMW (motor speed readings not available in this mode)
#define PWM_COMS_MDB

//define this to test the MDB
//#define MDB_test

#define ESC_PIN RC_PORTX04 // this pin corresponds to RB0 or J5 15 ont the UNO

#ifndef SETTERS_H
#define	SETTERS_H

void init_all_setters(void);
/* Function: init_all_setters()
 *
 * This function initializes all of the setters and the hardware they pertain to
 */

void init_ESC_pulse(void);
/* Function: init_ESC_pulse(void)
 *
 * Used to initialize ESC output module. Check the source to find the output pin.
 * Speed will be initialized to 0 rpm.
 */



void set_ESC_pulse(unsigned int pulse_width);

/* Function: set_ESC_pulse(unsigned int pulse_width)
 *
 * The brushless ESC (Electronic Speed Control) takes servo pulses which tell it
 * how fast to spin. The value for pulse width should be between 1000 (max reverse)
 * and 2000 (max forward), 1500 corresponding to zero speed. Anything above out
 * of bounds will be rounded up or down to either max forward or max reverse.
 *
 *
 */



void init_MDB(void);
/*Function init_MDB

 * This function just waits a bit so the MDB has time to start up.
 */

int16_t motor_command(int16_t command);
/*Function void motor_command(int16_t command)
 *
 * Input: int16_t command: duty cycle and direction. Between -1000 and 1000. Anythign
 * above or below will be rounded, except 0x8000 which does nothing, and is only
 * use for reading
 *
 * Output: Every time anything is sent to the MDB, it sends back it's current velocity in rpm
 * Negative values means reverse direction.

 * On every trasnmission this function sets up the SPI the way it needs it.
 */







#endif	/* SETTERS_H */

