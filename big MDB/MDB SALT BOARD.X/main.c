/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on August 7, 2014, 1:40 PM
 */
//#include <xc.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//_FOSCSEL(FNOSC_FRC);
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//_FWDT(FWDTEN_OFF);
//_FICD(JTAGEN_OFF & ICS_PGD1);
//
//
///*
// *
// */
//void MotorInit()
//{
//#if defined(__dsPIC33FJ64GS608__)
//	PTCONbits.PTEN = 0;
//
//	/* Setup for the Auxiliary clock to use the FRC as the REFCLK */
//	/* ((FRC * 16) / APSTSCLR) = (7.49 * 16) / 1 = 119.84 MHz */
//	ACLKCONbits.FRCSEL = 1; /* FRC is input to Auxiliary PLL */
//	ACLKCONbits.SELACLK = 1; /* Auxiliary Oscillator provides the clock
//	source */
//	ACLKCONbits.APSTSCLR = 1; /* Divide Auxiliary clock by 1 */
//	ACLKCONbits.ENAPLL = 1; /* Enable Auxiliary PLL */
//	while (ACLKCONbits.APLLCK != 1); /* Wait for Auxiliary PLL to Lock */
//
//	//setup PWM ports
//	PWMCON1bits.ITB = 0; /* PTPER provides the PWM time period value */
//	PWMCON2bits.ITB = 0; /* PTPER provides the PWM time period value */
//	PWMCON3bits.ITB = 0; /* PTPER provides the PWM time period value */
//	PWMCON4bits.ITB = 0; /* PTPER provides the PWM time period value */
//	PWMCON5bits.ITB = 0; /* PTPER provides the PWM time period value */
//	PWMCON6bits.ITB = 0; /* PTPER provides the PWM time period value */
//
//	IOCON1bits.PENH = 1;
//	IOCON2bits.PENH = 1;
//	IOCON3bits.PENH = 1;
//	IOCON4bits.PENH = 1;
//	IOCON5bits.PENH = 1;
//	IOCON6bits.PENH = 1;
//
//	//Setup desired frequency by setting period for 1:64 prescaler
//	PTPER = 10100;
//	PHASE1 = 0;
//	PHASE2 = 0;
//	PHASE3 = 0;
//	PHASE4 = 0;
//	PHASE5 = 0;
//	PHASE6 = 0;
//
//	PTCONbits.PTEN = 1;
//#endif
//
//}
//
//int main(int argc, char** argv) {
//
//    return (EXIT_SUCCESS);
//}

