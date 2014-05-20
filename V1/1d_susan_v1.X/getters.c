/* File: getters.c
 * Author: Dmitriy Rivkin
 *
 *
 *
 *
 * Created May 8, 2014


 */

#include "xc.h"
#include "serial.h"
#include "BOARD.h"
#include "getters.h"
#include <peripheral/spi.h>
#include <peripheral/incap.h>
#include <peripheral/timer.h>

#define SPISS 0x10000000 // enable proper operation of the SS bit for reading the encoder
#define REQUEST_ANGLE 0XFFFF

#define WAIT_TIME 1000

// THIS FUNCTION IS BLOCKING (but not for too long)
void wait(int cycles) {
    while (cycles > 0) {
        cycles--;
    }
}

void init_all_getters(void){
    init_encoder();
}
#ifdef SPI_ENC
void init_encoder(void) {
    // this part sets up the SPI interface\
    // channel 2 is the one for J8 on the UNO32
    unsigned int config = SPI_CON_MODE16 | SPI_CON_MSTEN | SPI_CON_SMP;
    // the last number is the clock divider
    SpiChnOpen(SPI_CHANNEL2, config, 1024);

    // this is the slave select pin it's controlled in software you need to bring
    // it high when you're not transmitting and low 10ns before you start transmitting
    // you also need to cycle it between characters
    TRISGbits.TRISG9 = 0;
    PORTGbits.RG9 = 1;

    PORTGbits.RG9 = 0;
    wait(WAIT_TIME);
    SpiChnPutC(SPI_CHANNEL2, REQUEST_ANGLE);
    SpiChnGetC(SPI_CHANNEL2);
    PORTGbits.RG9 = 1;
    // TRISGbits.TRISG9 = 0;
    //PORTGbits.RG9 = 1;
}

int get_encoder_angle(void) {
    int angle;
    //request the next angle and read the current angle simultaneously
    PORTGbits.RG9 = 0;
    wait(WAIT_TIME);
    SpiChnPutC(SPI_CHANNEL2, REQUEST_ANGLE);
    angle = SpiChnGetC(SPI_CHANNEL2);
    PORTGbits.RG9 = 1;

    //get rid of bits 15 and 14 of angle, they're transmission details
    angle = angle & 0x00003fff;

    return angle;
}
#endif

#ifndef SPI_ENC

void init_encoder(void) {
     //Clear interrupt flag
    mIC1ClearIntFlag();

    // Setup Timer 3 with long period, no clock scaling
    OpenTimer3(T3_ON | T3_PS_1_4, 0xffff);

    // Enable Input Capture Module 1
    // - Capture Every edge
    // - Enable capture interrupts
    // - Use Timer 3 source
    // - Capture rising edge first
    OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_RISE | IC_ON );

     // Wait for Capture events
    while( !mIC1CaptureReady() ) ;
}



 int get_encoder_angle(void) {
    // rising and fallind edge times
    int up1,up2,d1;

    // the angle expressed by the pwm
    int angle;

    //reset timer
    WriteTimer3(0);
    //printf("timer 3: %d\n",ReadTimer3());
    //turn on input capture
    OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_RISE |IC_CAP_16BIT| IC_ON );

    //wait for first data to be ready
    while( !mIC1CaptureReady() ) ;
    //first rising edge
    up1=mIC1ReadCapture();

    //wait for falling edge
    while( !mIC1CaptureReady() ) ;
    d1=mIC1ReadCapture();

    // wait for next rising edge
    while( !mIC1CaptureReady() ) ;
    up2=mIC1ReadCapture();

    //turn off input capture
    OpenCapture1( IC_EVERY_EDGE | IC_INT_1CAPTURE | IC_TIMER3_SRC | IC_FEDGE_RISE | IC_CAP_16BIT | IC_OFF );
    //printf("timer 3: %d\n",ReadTimer3());

    // The values come from a 16 bit timer therefore the lower 15 bits (since it's signed)
    // are just noise. Shift them out for the bottom, leave them in for the top of the division.
    int temp=up2-up1;
    temp=temp >> 15;

    angle=(d1-up1)/temp;
    printf("period %d\n",up2-up1);
    printf("up1 %d\n d1 %d\n up2 %d\n",up1,d1,up2);

    return angle;
}


#endif


#ifdef TEST_ENCODER

#define TIMER_VAL 0xFFFF

void timer_init(void) {
    T2CON = 0x0;
    TMR2 = 0x0;
    PR2 = TIMER_VAL;

    IPC2SET = 0x0000000D;
    IFS0CLR = 0x00000100;
    IEC0SET = 0x00000100;
    //turn on timer and set prescaler
    T2CONSET = 0x8070;
}

int angle = 0;

void __ISR(_TIMER_2_VECTOR, ipl3) Timer2Handler(void) {
    T2CONSET = 0x8000;
    printf("timer overrun \n");


    angle = get_encoder_angle();
    printf("%d\n", angle);



    // clear timer interrupt
    IFS0CLR = 0x00000100;

}





void main() {
    BOARD_Init();
    timer_init();
    init_encoder();
}

#endif

