/* 
 * File:   SPIcoms.h
 * Author: Dmitriy
 *
 * The SPI module accepts commands from the master and passes them to the commutation
 * library. It also passes along the speed and direction to the master.
 *
 *
 * Created on July 22, 2014, 10:05 AM
 */



#ifndef SPICOMS_H
#define	SPICOMS_H

//sets up the SPI module
void SPIinit(void);


// this is used to send back debugging information and not for use during actuall
// operation. This pops messages onto a queue and sends them off when the master
// transmits something. Normally we will always be sending the most up to date
// speed information back to the master and will have no use for queues
void SPI_send(int16_t to_send);



#endif	/* SPICOMS_H */

