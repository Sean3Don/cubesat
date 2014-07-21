/*
 * File:   cubesat_SPI_bus_library.h
 * Author: Dmitriy
 *
 * Created on July 18, 2014, 12:45 PM
 *
 * About:
 *
 * Most communication on the cubesat (AHRS, motor driver boards, encoder) takes
 * place over SPI. The UNO32 functions as the master in all cases. The peripherals
 * all connect to the same data and clock lines, but each one has it's own slave
 * select line, which allows the UNO to talk to one at a time.
 *
 * Each peripheral has slightly different requirements for how the SPI should be set
 * up, so every time we talk to a new peripheral we have to re-initialize it.
 *
 * The kind of communication we have with each peripheral is different too. For
 * example, the encoder sends back a single 16 bit number, the AHRS can send
 * back a variety of information, and the MDB (motor driver boards) send information
 * to the UNO but are also controlled via commands from the UNO. As such, the
 * library provides a different communication function for each peripheral, and
 * each function will have a varying degree of flexibility depending on what
 * communication needs it satisfies.
 *
 * Initializations for the peripherals (such as waiting a while for them to start
 * up), are not done in the SPI library.
 *
 *
 *
 */

//#define TEST_SPI

#ifndef CUBESAT_SPI_BUS_LIBRARY_H
#define	CUBESAT_SPI_BUS_LIBRARY_H



/* Function: float * get_AHRS_data(unsigned int  data_request, unsigned int byte_num)
 *
 * Inputs:
 * unsigned int data request : This is the code what tells the AHRS what data to send back
 * unsigned int byte_num : The number of bytes that the AHRS data you want takes up
 *
 *
 * Output: A pointer to an array of unsigned ints which contains the raw data from
 * the AHRS. Only the least significant 8 bits of the each unsigned int are valid.
 *

 */

uint32_t * get_AHRS_data(unsigned int data_request, unsigned int byte_num);


/* Function: void send_AHRS_command(char command)
 *
 * Use this function to send commands to the AHRS, such as tare.
 *
 * char command: this is a command byte
 *
 * Refer to the tables at the end of the AHRS user's manual.
 */
void send_AHRS_command(unsigned int command);


/*Function: int get_motor_velocity();
 *
 * This function returns an int between 7000 and -7000 indicating the RPM of the
 * motor. Negative means the motor is spinning in reverse.
 *
 *  */
int get_motor_velocity();

/*Function: void set_motor_duty_cycle_and_direction(int duty_and_dir);
 *
 * Inputs: duty_and_dir is an int between -1000 and 1000. Negative values
 * mean spinning in reverse. 1000 is 100% duty cycle, 0 is 0% duty cycle.

 */
void set_motor_duty_cycle_and_direction(int duty_and_dir);



#endif	/* CUBESAT_SPI_BUS_LIBRARY_H */

