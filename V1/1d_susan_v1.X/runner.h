/* 
 * File:   runner.h
 * Author: Dmitriy
 *
 * This code runs the getting, control, and setting, at a sample rate which is
 * passed as a variable
 *
 * Created on May 8, 2014, 5:59 PM
 */

#ifndef RUNNER_H
#define	RUNNER_H

// if RUNTEST is defined then all the function does is toggle a pin at the test
//frequency (there is a single toggle every test period, not a full cycle)
//#define RUNTEST
#define RUNTESTFREQ 100


void run(int samp_freq);
/*
 * FUNCTION: run(int samp_freq);
 *
 * input: samp_freq -- control system frequency (Hz)
 *
 * this function calls all the getters, then the control system, and then all the
 * setters, at a frequency given by samp freq.

 */





#endif	/* RUNNER_H */

