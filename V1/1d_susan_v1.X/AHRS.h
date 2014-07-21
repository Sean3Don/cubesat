/* 
 * File:   AHRS.h
 * Author: Dmitriy
 *
 * This file implements the SPI interface between the AHRS (Attitude & Heading Reference System)
 * and the uno32.
 * Created on May 15, 2014, 2:38 PM
 */
// define AHRS_TEST to run test harness
 #define AHRS_TEST


#ifndef AHRS_H
#define	AHRS_H

void AHRS_init();
/* function: AHRS_init()
 *
 * Initializes the AHRS SPI interface. Also tares the AHRS with the current orientation.

 */


float AHRS_get_yaw(void);
/*
 * function: AHRS_get_yaw
 *
 * returns the yaw angle from the AHRS

 */


#endif	/* AHRS_H */

