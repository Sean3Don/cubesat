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

//define this if you need to run the ESC calibration routine
//#define CALIBRATE_ESC

//define this if you want to play with the ESC
//#define ESC_TEST


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










#endif	/* SETTERS_H */

