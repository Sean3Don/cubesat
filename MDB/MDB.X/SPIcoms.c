#include <xc.h>
#include "commutation.h"
#include "SPIcoms.h"
#include <stdint.h>
#include <stdlib.h>
#include "pps.h"

//#define SPITEST

#define LED1   LATCbits.LATC1
#define LED2   LATCbits.LATC2

int16_t send_buf[8];
int16_t head = 0;
int16_t tail = 0;

void SPI_send(int16_t to_send) {
    send_buf[tail] = to_send;
    tail++;
    if (tail == 8)
        tail = 0;
}

void pop_to_outbox(void) {
    if (head != tail) {
        SPI1BUF = send_buf[head];
        head++;
        if (head == 8)
            head = 0;
    } else {
        SPI1BUF = 0x8000; //indicate nothing more to transmit
    }
}

void SPIinit(void) {
    //    //first, we map the pins of SPI2 to the pins SPI1 was on, except that now
    //    // the pin labeled SDA1 is SS2
    //
    //    //unlock the ability to change mapping
    //    OSCCONL = 0x46;
    //    OSCCONL = 0x57;
    //    OSCCONbits.IOLOCK = 0;
    //
    //    //inputs:
    //    //SDI2 gets mapped to RPI25
    //    RPINR22bits.SDI2R = 0b0011001;
    //
    //    // SCK2 get mapped to RPI51
    //    RPINR22bits.SCK2INR = 0b0110011;
    //
    //    //SS2 gets mapped to RPI52
    //    RPINR23bits.SS2R = 0b0110100;
    //
    //    //outputs:
    //    //RP20 gets mapped to SDO2
    //    RPOR0bits.RP20R = 0b0010000;
    //
    //    //lock the ability to change mapping
    //    OSCCONL = 0x46;
    //    OSCCONL = 0x57;
    //    OSCCONbits.IOLOCK = 1;


    PPSUnLock;
    PPSInput(IN_FN_PPS_SDI2, IN_PIN_PPS_RPI25);
    PPSInput(RPINR22bits.SCK2INR, IN_PIN_PPS_RPI51);
    PPSInput(IN_FN_PPS_SS2, IN_PIN_PPS_RPI52);
    PPSOutput(OUT_FN_PPS_SDO2, OUT_PIN_PPS_RP20);
    PPSLock;


    // we now set the desired inputs as inputs and desired outputs as outputs
    //probably not necessary but potentially safer in case we messed up the re-mapping
    TRISAbits.TRISA9 = 1; //SDI
    TRISCbits.TRISC3 = 1; //SCK
    TRISCbits.TRISC4 = 1; // SDA1 which should now be SS2
    TRISAbits.TRISA4 = 0; //SDO

    //JUST FOR TESTING
    //    PORTAbits.RA9 = 1;
    //    PORTCbits.RC3 = 1;
    //    PORTCbits.RC4 = 1;
    //   PORTAbits.RA4 = 1;

    //Now that the pins are assigned, we initialize SPI2

    SPI2BUF = 0;
    IFS2bits.SPI2IF = 0; // clear interrupt flag
    IEC2bits.SPI2IE = 0; //disable interrupts

    //    //SPI1CON1 register settings
    SPI2CON1bits.DISSCK = 0; // enable internal serial clock ???
    SPI2CON1bits.DISSDO = 0; //SDO pin controlled by module
    SPI2CON1bits.MODE16 = 1; // set 16 bit mode

    SPI2CON1bits.CKE = 1; // data changes on transition of clock from active to idle
    SPI2CON1bits.CKP = 0; // Idle state is low level, active state is high level
    SPI2CON1bits.MSTEN = 0; // Master mode disabled

    SPI2CON1bits.SMP = 0; // must be cleared in slave mode according to datasheet

    //SPI2CON2bits.FRMEN =0;
    SPI2CON1bits.SSEN = 1; // only receive when ssx pin is low

    SPI2STATbits.SPIROV = 0; // no receive overflow has occured
    SPI2STATbits.SPIEN = 1; //Enable SPI module

    IFS2bits.SPI2IF = 0; // clear interrupt flag
    IEC2bits.SPI2IE = 1; //enable interrupts





    //quasi-working code, the SS1 line pin is messed up. therefore
    //we  map spi2 to spi1 and SDA = SS2

    //    SPI1BUF = 0;
    //    TRISBbits.TRISB0 = 1; // Set the SS pin as input
    //
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
    //    SPI1CON1bits.SMP = 0; // must be cleared in slave mode according to datasheet
    //
    //    SPI1CON2bits.FRMEN =0;
    //    SPI1CON1bits.SSEN = 1; // only receive when ssx pin is low
    //
    //    SPI1STATbits.SPIROV = 0; // no receive overflow has occured
    //    SPI1STATbits.SPIEN = 1; //Enable SPI module
    //
    //    // Interrupt controller settings
    //
    //    IFS0bits.SPI1IF = 0; // clear the interrupt flag
    //    IEC0bits.SPI1IE = 1; // enable the interrupt





}

#ifdef TORQUE_COMMAND

void __attribute((__interrupt__, no_auto_psv)) _SPI2Interrupt(void) {
    int16_t command = SPI2BUF;
    update_torque_command(command);
    IFS2bits.SPI2IF = 0; // clear interrupt flag
}


#endif


#ifdef PWMCOMMAND

void __attribute((__interrupt__, no_auto_psv)) _SPI2Interrupt(void) {
    int16_t command = SPI2BUF; // read what came in
    //pop_to_outbox();
    update_duty_cycle_and_direction(command);


#ifdef TEST_COMMAND
    //only send current command if a command is sent, not a readout request
    //if(command != 0x8000)
    //SPI_send(get_current_command());
    SPI1BUF = get_current_command();
#endif

    //JUST FOR TESTING
    if (command == 500) {
        LED1 = 1;
    } else {
        LED1 = 0;
    }

    IFS2bits.SPI2IF = 0; // clear interrupt flag

}
#endif

#ifdef SPITEST
_FOSCSEL(FNOSC_FRC & IESO_OFF & PWMLOCK_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);
_FICD(ICS_PGD1 & JTAGEN_OFF);

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

int main(void) {
    clockInit();
    SPIinit();
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    LED2 = 1;
    LED1 = 1;
    while (1);
}

#endif
