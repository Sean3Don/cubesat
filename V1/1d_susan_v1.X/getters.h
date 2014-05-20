/* 
 * File:   getters.h
 * Author: Dmitriy
 *
 * These functions read data from sensors. Initialization functions for getters are
 * also included in this library.
 * Created on May 8, 2014, 4:13 PM
 */

//define this if you want to test the encoder
// the test should spit out encoder values indefinitely
//#define TEST_ENCODER

// define this if you want the encoder to be read through SPI. Otherwise it uses
// pwm and input capture.
//#define SPI_ENC


#ifndef GETTERS_H
#define	GETTERS_H

void init_all_getters(void);
/*
 *  Function: init_all_getters(void)
 * 
 * Initializes all of the sensing functionalitie
 */

void init_encoder(void);
/*
 * Function: init_encoder(void)
 * 
 * Data is gathered from the encoder via SPI. This sets up the spi interface and
 * prepares for aquisition of angle data from the encoder.
 */

int get_encoder_angle(void);
/*
 * Function: get_encoder_angle(void)
 *
 * Returns: An unsigned int, the lowest 14 bits contain the absolute angle read
 * from the encoder
 */



#endif	/* GETTERS_H */

