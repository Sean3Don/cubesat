/*
 * File:   runner.h
 * Author: Dmitriy
 *
 * This code runs the getting, control, and setting, at a sample rate which is
 * passed as a variable
 *
 * Created on May 8, 2014, 5:59 PM
 */

#include "xc.h"
#include "serial.h"
#include "BOARD.h"
#include "runner.h"
#include "getters.h"
#include "setters.h"
#include "Control.h"
#include <peripheral/timer.h>
#include <peripheral/ports.h>
/*
 * 
 */

// what this function really does is initialize the timer interrupt to the correct frequency

void run(int samp_freq) {
    // get the peripheral clock frequency
    unsigned int Pclock = BOARD_GetPBClock();
    unsigned int timer_per = (Pclock / 256) / samp_freq;

    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, timer_per);
    INTEnableSystemMultiVectoredInt();
    // set up the timer interrupt with a priority of 2
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_1);


}


// this ISR is the part that really runs the show

void __ISR(_TIMER_1_VECTOR, ipl1) Timer1Handler(void) {
    // clear the interrupt flag
    mT1ClearIntFlag();

#ifndef RUNTEST

    struct inputs in;
    struct outputs out;
    //in.enc_angle = get_encoder_angle();
    in.AHRS_angle = get_AHRS_angle();
    Control(&in,&out);
    set_ESC_pulse(out.pulse_width);

    // clear the interrupt flag
#endif


#ifdef RUNTEST
    mPORTDToggleBits(BIT_0);
#endif

}

#ifdef RUNTEST

void main() {
    mPORTDSetPinsDigitalOut(BIT_0);
    run(RUNTESTFREQ);
}

#endif