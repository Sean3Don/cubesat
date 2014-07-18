

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <uart.h>
#include "adc.h"

_FOSCSEL(FNOSC_FRC & IESO_OFF & PWMLOCK_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);
_FICD(ICS_PGD1 & JTAGEN_OFF);

#define LED1   LATCbits.LATC1
#define LED2   LATCbits.LATC2
#define RESET3 LATBbits.LATB11
#define RESET2 LATBbits.LATB13
#define RESET1 LATBbits.LATB15
#define FORWARD 1
#define REVERSE 0

void clockInit() {
    // 140.03 MHz VCO  -- 70 MIPS
    PLLFBD = 74;
    CLKDIVbits.PLLPRE = 0;
    CLKDIVbits.PLLPOST = 0;

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);

    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);
}

void timersInit(void) {

    T2CONbits.TON = 0; // Ensure Timer 2 is in reset
    T2CONbits.TCS = 0; // internal clock
    T1CONbits.TGATE = 0; // disable gated mode
    T2CONbits.TCKPS = 0b11; // prescaler 256
    TMR2 = 0; // clear timer register
    PR2 = 0x0001; // period

    IPC1bits.T2IP = 0X01; // set interrupt priority
    IFS0bits.T2IF = 0; // clear inteerupt flag
    IEC0bits.T2IE = 1; // enable inteerupt
    T2CONbits.TON = 1; // start timer



    //T2CONbits.TCKPS = 0b11; // Set Timer2 control register TON = 1, TCKPS1:TCKPS0 = 0b11 (1:256 prescaler)
    // Use system clock Fosc/2 = 40 MHz. Result is a 156,250 Hz clock.
}





//Sets up the pins and interrups for the hall effect sensors. These pins will be
//polled and this will allow us to determine when to switch

void hallInit(void) {
    //hall a
    TRISCbits.TRISC6 = 1;

    // hall b
    TRISCbits.TRISC7 = 1;

    // hall c
    TRISCbits.TRISC8 = 1;
}

//set up pwm modules

void pwmInit(void) {
    /* Set PWM Period on Primary Time Base */
    PTPER = 1000;
    /* Set Phase Shift */
    PHASE1 = 0;
    //SPHASE1 = 0;
    PHASE2 = 0;
    //SPHASE2 = 0;
    PHASE3 = 0;
    //SPHASE3 = 0;

    /* Set Duty Cycles */
    PDC1 = 500;
    //SDC1 = 1000;
    PDC2 = 500;
    //SDC2 = 1000;
    PDC3 = 500;
    //SDC3 = 1000;


    //set the low side IO pins to be software controlled. Setting one
    // of these low causes the coil to go to high impedance
    IOCON1bits.PENL = 0;
    IOCON2bits.PENL = 0;
    IOCON3bits.PENL = 0;

    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB15 = 0;

    RESET1 = RESET2 = RESET3 = 1;
    /* Set Dead Time Values */
    DTR1 = DTR2 = DTR3 = 0;
    ALTDTR1 = ALTDTR2 = ALTDTR3 = 0;
    PWMCON1 = PWMCON2 = PWMCON3 = 0x0000;
    /* Configure Faults */
    FCLCON1 = FCLCON2 = FCLCON3 = 0x0003;
    /* 1:1 Prescaler */
    PTCON2 = 0x0000;

    PTCON = 0x8000;
}

// duty cycle from 0 to 1000, direction = 1 for forward and 0 for reverse

void block_com(int duty_cycle, int direction) {
    unsigned int dc1 = duty_cycle;
    unsigned int dc2 = duty_cycle;
    unsigned int dc3 = duty_cycle;

    unsigned int r1 = 1;
    unsigned int r2 = 1;
    unsigned int r3 = 1;

//doesnt spin
//    int hallA = PORTCbits.RC6;
//    int hallB = PORTCbits.RC7;
//    int hallC = PORTCbits.RC8;

    //spins and clicks, needs a kick
//    int hallA = PORTCbits.RC8;
//    int hallB = PORTCbits.RC7;
//    int hallC = PORTCbits.RC6;

    // doesn't spin
//    int hallA = PORTCbits.RC8;
//    int hallB = PORTCbits.RC6;
//    int hallC = PORTCbits.RC7;

    //doesn't spin
//    int hallA = PORTCbits.RC7;
//    int hallB = PORTCbits.RC8;
//    int hallC = PORTCbits.RC6;

    //spins and clicks at 100 not at 300
    int hallA = PORTCbits.RC6;
    int hallB = PORTCbits.RC8;
    int hallC = PORTCbits.RC7;

    char halls = 0x00;
    halls = hallA << 2 | hallB << 1 | hallC;
    //LED1=1;
    //    if (hallA){
    //        LED1=1;
    //    }else{
    //        LED1=0;
    //    }

    switch (halls) {
        case 0b110:
            r1 = 0;

            dc3 = 0;

            break;

        case 0b010:
            r3 = 0;
            dc1 = 0;
            break;

        case 0b011:
            r2 = 0;
            dc1 = 0;
            break;

        case 0b001:
            r1 = 0;
            dc2 = 0;
            break;

        case 0b101:
            r3 = 0;
            dc2 = 0;
            break;

        case 0b100:
            r2 = 0;
            dc3 = 0;
            break;

        default:
            dc1 = dc2 = dc3 = 0;
            break;
    }
    if (direction == REVERSE) {
        if (dc1) {
            dc1 = 0;
        } else {
            dc1 = duty_cycle;
        }
        if (dc2) {
            dc2 = 0;
        } else {
            dc2 = duty_cycle;
        }
        if (dc3) {
            dc3 = 0;
        } else {
            dc3 = duty_cycle;
        }
    }

    RESET1 = r1;
    RESET2 = r2;
    RESET3 = r3;

    PDC1 = dc1;
    PDC2 = dc2;
    PDC3 = dc3;

}



// timer interrupts call the function which reads the halls and re-evaluates
// which coils to turn on

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    //LED1 ^= 1;
    block_com(300, FORWARD);
    // PR2 = 0x0001; // Load the timer period value with 1

    TMR2 = 0x0000;
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
}

int main(void) {

    clockInit();
    hallInit();
    pwmInit();
    timersInit();
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    //LED2 = 1;
    LED1 = 0;
    while (1);
}

