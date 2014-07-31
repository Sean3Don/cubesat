
//#define RUNMAIN

#ifdef RUNMAIN

#include <xc.h>
#include <string.h>
#include <stdint.h>
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
#define SR 753 // *10^-8*60, SAMPLE RATE WAS NOT MEASURED VERY WELL, GET BETTER SAMPLE RATE MEASUREMENT

int32_t numerator=500000000; // 1/12 (rotations in a hall step) *10^8


int16_t to_write = 0;
int16_t command = 0;
int16_t duty_cycle = 0;
int16_t direction = 0;
int16_t current_speed = 0;

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

//void timersInit(void) {
//
//    T2CONbits.TON = 0; // Ensure Timer 2 is in reset
//    T2CONbits.TCS = 0; // internal clock
//    T1CONbits.TGATE = 0; // disable gated mode
//    T2CONbits.TCKPS = 0b11; // prescaler 256
//    TMR2 = 0; // clear timer register
//    PR2 = 0x0001; // period
//
//    IPC1bits.T2IP = 0X01; // set interrupt priority
//    IFS0bits.T2IF = 0; // clear inteerupt flag
//    IEC0bits.T2IE = 1; // enable inteerupt
//    T2CONbits.TON = 1; // start timer
//
//
//
//    //T2CONbits.TCKPS = 0b11; // Set Timer2 control register TON = 1, TCKPS1:TCKPS0 = 0b11 (1:256 prescaler)
//    // Use system clock Fosc/2 = 40 MHz. Result is a 156,250 Hz clock. Testing shows that real clock period is closer
//    //to 0.00000753371s or a frequency of 132736.725996 Hz, though this was not measured very scientifically.
//}





//Sets up the pins and interrups for the hall effect sensors. These pins will be
//polled and this will allow us to determine when to switch
//char oldHalls;
//
//void hallInit(void) {
//    //hall a
//    TRISCbits.TRISC6 = 1;
//
//    // hall b
//    TRISCbits.TRISC7 = 1;
//
//    // hall c
//    TRISCbits.TRISC8 = 1;
//
//    int hallA = PORTCbits.RC6;
//    int hallB = PORTCbits.RC8;
//    int hallC = PORTCbits.RC7;
//
//    char halls = 0x00;
//    halls = hallA << 2 | hallB << 1 | hallC;
//
//    oldHalls = halls;
//}

//set up pwm modules

//void pwmInit(void) {
//    /* Set PWM Period on Primary Time Base */
//    PTPER = 1000;
//    /* Set Phase Shift */
//    PHASE1 = 0;
//    //SPHASE1 = 0;
//    PHASE2 = 0;
//    //SPHASE2 = 0;
//    PHASE3 = 0;
//    //SPHASE3 = 0;
//
//    /* Set Duty Cycles */
//    PDC1 = 500;
//    //SDC1 = 1000;
//    PDC2 = 500;
//    //SDC2 = 1000;
//    PDC3 = 500;
//    //SDC3 = 1000;
//
//
//    //set the low side IO pins to be software controlled. Setting one
//    // of these low causes the coil to go to high impedance
//    IOCON1bits.PENL = 0;
//    IOCON2bits.PENL = 0;
//    IOCON3bits.PENL = 0;
//
//    TRISBbits.TRISB13 = 0;
//    TRISBbits.TRISB11 = 0;
//    TRISBbits.TRISB15 = 0;
//
//    RESET1 = RESET2 = RESET3 = 1;
//    /* Set Dead Time Values */
//    DTR1 = DTR2 = DTR3 = 0;
//    ALTDTR1 = ALTDTR2 = ALTDTR3 = 0;
//    PWMCON1 = PWMCON2 = PWMCON3 = 0x0000;
//    /* Configure Faults */
//    FCLCON1 = FCLCON2 = FCLCON3 = 0x0003;
//    /* 1:1 Prescaler */
//    PTCON2 = 0x0000;
//
//    PTCON = 0x8000;
//}
//
//uint32_t ticks = 1;
//
//
//void get_speed_and_direction(char currHalls) {
//    if (currHalls != oldHalls) {
//        switch (oldHalls) {
//            case 0b110:
//                if (currHalls == 0b010)
//                    direction = FORWARD;
//                else
//                    direction = REVERSE;
//                break;
//
//            case 0b010:
//                if (currHalls == 0b011)
//                    direction = FORWARD;
//                else
//                    direction = REVERSE;
//                break;
//
//            case 0b011:
//                if (currHalls == 0b001)
//                    direction = FORWARD;
//                else
//                    direction = REVERSE;
//                break;
//
//            case 0b001:
//                if (currHalls == 0b101)
//                    direction = FORWARD;
//                else
//                    direction = REVERSE;
//
//                break;
//
//            case 0b101:
//                if (currHalls == 0b100)
//                    direction = FORWARD;
//                else
//                    direction = REVERSE;
//
//                break;
//
//            case 0b100:
//                if (currHalls == 0b110 )
//                    direction=FORWARD;
//                else
//                    direction=REVERSE;
//                break;
//
//            default:
//                break;
//        }
//        current_speed = numerator/(ticks*753);
//        ticks = 1;
//
//    } else {
//        ticks++;
//    }
//}


// duty cycle from 0 to 1000, direction = 1 for forward and 0 for reverse

//void block_com(int duty_cycle, int direction) {
//    unsigned int dc1 = duty_cycle;
//    unsigned int dc2 = duty_cycle;
//    unsigned int dc3 = duty_cycle;
//
//    unsigned int r1 = 1;
//    unsigned int r2 = 1;
//    unsigned int r3 = 1;
//
//    //doesnt spin
//    //    int hallA = PORTCbits.RC6;
//    //    int hallB = PORTCbits.RC7;
//    //    int hallC = PORTCbits.RC8;
//
//    //spins and clicks, needs a kick
//    //    int hallA = PORTCbits.RC8;
//    //    int hallB = PORTCbits.RC7;
//    //    int hallC = PORTCbits.RC6;
//
//    // doesn't spin
//    //    int hallA = PORTCbits.RC8;
//    //    int hallB = PORTCbits.RC6;
//    //    int hallC = PORTCbits.RC7;
//
//    //doesn't spin
//    //    int hallA = PORTCbits.RC7;
//    //    int hallB = PORTCbits.RC8;
//    //    int hallC = PORTCbits.RC6;
//
//
//
//    int hallA = PORTCbits.RC6;
//    int hallB = PORTCbits.RC8;
//    int hallC = PORTCbits.RC7;
//
//    char halls = 0x00;
//    halls = hallA << 2 | hallB << 1 | hallC;
//    //LED1=1;
//    //    if (hallA){
//    //        LED1=1;
//    //    }else{
//    //        LED1=0;
//    //    }
//
//    switch (halls) {
//        case 0b110:
//            r1 = 0;
//
//            dc3 = 0;
//
//            break;
//
//        case 0b010:
//            r3 = 0;
//            dc1 = 0;
//            break;
//
//        case 0b011:
//            r2 = 0;
//            dc1 = 0;
//            break;
//
//        case 0b001:
//            r1 = 0;
//            dc2 = 0;
//            break;
//
//        case 0b101:
//            r3 = 0;
//            dc2 = 0;
//            break;
//
//        case 0b100:
//            r2 = 0;
//            dc3 = 0;
//            break;
//
//        default:
//            dc1 = dc2 = dc3 = 0;
//            break;
//    }
//    if (direction == REVERSE) {
//        if (dc1) {
//            dc1 = 0;
//        } else {
//            dc1 = duty_cycle;
//        }
//        if (dc2) {
//            dc2 = 0;
//        } else {
//            dc2 = duty_cycle;
//        }
//        if (dc3) {
//            dc3 = 0;
//        } else {
//            dc3 = duty_cycle;
//        }
//    }
//
//    RESET1 = r1;
//    RESET2 = r2;
//    RESET3 = r3;
//
//    PDC1 = dc1;
//    PDC2 = dc2;
//    PDC3 = dc3;
//
//    get_speed_and_direction(halls);
//}
//
//void SPI_init(void) {
//    SPI1BUF = 0;
//    IFS0bits.SPI1IF = 0; // clear the interrupt flag
//    IEC0bits.SPI1IE = 0; //disable interrups
//
//    //SPI1CON1 register settings
//    SPI1CON1bits.DISSCK = 0; // enable internal serial clock ???
//    SPI1CON1bits.DISSDO = 0; //SDO pin controlled by module
//    SPI1CON1bits.MODE16 = 1; // set 16 bit mode
//
//    SPI1CON1bits.CKE = 1; // data changes on transition of clock from active to idle
//    SPI1CON1bits.CKP = 0; // Idle state is low level, active state is high level
//    SPI1CON1bits.MSTEN = 0; // Master mode disabled
//
//    SPI1CON1bits.SSEN = 1; // only receive when ssx pin is low
//
//    SPI1STATbits.SPIROV = 0; // no receive overflow has occured
//    SPI1STATbits.SPIEN = 1; //Enable SPI module
//
//    // Interrupt controller settings
//
//    IFS0bits.SPI1IF = 0; // clear the interrupt flag
//    IEC0bits.SPI1IE = 1; // enable the interrupt
//
//
//}

//// this function looks at the current speed of the motor and the duty cycle command
////coming from the master. If the command would put more than 12 volts (but really
////11 for safety) accross the coil (back emf + vbat*duty cycle), the command is reduced
//// to something which produces only 11 volts. Back emf is calculated based on current motor
////speed and motor constants
//
//int16_t safety_check(int16_t command) {
//    return command;
//}
//
////updates the desired speed based on the command. Command must be between 1000
//// and -1000. Anything else will be rounded, except 0x8000, which is ignored
//
//void update_duty_cycle(int16_t command) {
//    if (command == 0x8000) { // this is means master just wants to read speed, do nothing
//        return;
//    }
//    if (command > 1000) {
//        command = 1000;
//    }
//    if (command<-1000) {
//        command = -1000;
//    }
//    command = safety_check(command); // perform a safety check to not burn motor
//
//    if(command < 0){
//        duty_cycle = -command;
//        direction = REVERSE;
//    }else{
//        duty_cycle = command;
//        direction = FORWARD;
//    }
//}
//
//void __attribute((__interrupt__, no_auto_psv)) _SPI1Interrupt(void) {
//    command = SPI1BUF; // read what came in
//    SPI1BUF = to_write; // to_write should get passed along to the master on next transmission
//    update_duty_cycle(command);
//
//    IFS0bits.SPI1IF = 0; // clear interrupt flag
//
//}




// timer interrupts call the function which reads the halls and re-evaluates
// which coils to turn on

//// the behavior is commanded by the master over SPI. If the current SPI command is 0x8000, this means
//// no change, the master is reading the speed. A command between 100
//uint32_t tt = 0;
//
//void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
//    //LED1 ^= 1;
//
//    //block_com(300, FORWARD);
//
//
//    //following code used for testing the interrupt frequency
//    // to make sure it's what we think it is (156,250 hz), we will make it count to
//    // 60*156250 and if it takes a minute we'll know it's good
//
//    tt++;
//    if (tt > 8203125) {
//        LED1 = 0;
//    }
//
//
//
//    TMR2 = 0x0000;
//    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
//
//
//}

//SPI reception is interrupt driven

int main(void) {

    clockInit();


    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    //LED2 = 1;
    LED1 = 1;
    while (1);
}

#endif