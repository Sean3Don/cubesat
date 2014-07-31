#include <xc.h>
#include <string.h>
#include <stdint.h>
#include "stdio.h"
#include <stdlib.h>
#include <uart.h>
#include "adc.h"
#include "math.h"
#include "commutation.h"

#define LED1   LATCbits.LATC1
#define LED2   LATCbits.LATC2
#define RESET3 LATBbits.LATB11
#define RESET2 LATBbits.LATB13
#define RESET1 LATBbits.LATB15

#define DISC_THRESH 2000 //Data points which are this far from the average will not be included in the average. Good for 
// keeping clean numbers, but make sure it's faster than the acceleration of the motor, otherwise
// the speed gets stuck. This should be adjusted when the reaction wheel is added since the motor
// wont be able to accelerate as fast.

// the extra factor of 10 is to keep some precision for the duty cycle
#define VmCmax 58080 // Vmax = 11V VmCmax = 10*11V*(1000 mv/V) /(1.895 mv/rpm)
#define VbattCmax 63  // Vbatt=12V VbattCmax = 12V*(1000 mv/V)/((1.895 mv/rpm)*1000) the last factor of 1000 is because duty cycle goes up to 1000

#define HALLA  PORTCbits.RC6
#define HALLB  PORTCbits.RC8
#define HALLC  PORTCbits.RC7
#define FORWARD 1
#define REVERSE 0

//commanded:
int16_t duty_cycle = 0;
int16_t drive_direction = 1;
int16_t current_command = 0; // this is what is currently being executed

//measured:
int16_t direction = 1;
int16_t current_speed = 0;
uint32_t ticks = 1;

int32_t numerator = 638624; // (60 s/m *127725 ticks/s)/(12 halls/rotation)
char oldHalls;

void commutationInit(void) {
    T2CONbits.TON = 0; // Ensure Timer 2 is in reset
    T2CONbits.TCS = 0; // internal clock
    T2CONbits.TGATE = 0; // disable gated mode
    T2CONbits.TCKPS = 0b11; // prescaler 256
    TMR2 = 0; // clear timer register
    PR2 = 0x0001; // period

    IPC1bits.T2IP = 0X01; // set interrupt priority
    IFS0bits.T2IF = 0; // clear inteerupt flag
    IEC0bits.T2IE = 1; // enable inteerupt
    T2CONbits.TON = 1; // start timer



    //T2CONbits.TCKPS = 0b11; // Set Timer2 control register TON = 1, TCKPS1:TCKPS0 = 0b11 (1:256 prescaler)
    // Use system clock Fosc/2 = 40 MHz. Result is a 156,250 Hz clock. Testing shows that real clock period is closer
    //to 0.00000753371s or a frequency of 132736.725996 Hz, though this was not measured very scientifically.
}

void hallsInit(void) {
    //hall a
    TRISCbits.TRISC6 = 1;

    // hall b
    TRISCbits.TRISC7 = 1;

    // hall c
    TRISCbits.TRISC8 = 1;

    int hallA = HALLA;
    int hallB = HALLB;
    int hallC = HALLC;

    char halls = 0x00;
    halls = hallA << 2 | hallB << 1 | hallC;

    oldHalls = halls;
}

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

int16_t get_current_command(void) {
    return current_command;
}

//Eight point moving average filter which discards data points which are too
// far (1000 rpm) from the average
int16_t last8[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int32_t sum = 0;
int i = 0;

int16_t MAV_filt(int16_t datum) {
    int16_t avg = sum >> 3;
    int16_t diff = avg - datum;
    if (abs(diff) > DISC_THRESH) { // if the jump reported is too large ingnore it
        datum = avg;
    }
    sum = sum - last8[i] + datum; //update the sum
    last8[i] = datum;
    i++;
    if (i == 8) {
        i = 0;
    }
    return sum >> 3; //return sum divided by 8 or average
}

int dirsign;

void get_speed_and_direction(char currHalls) {

    if (currHalls != oldHalls) {
        LED2 ^= 1;
        switch (oldHalls) {
            case 0b110:
                if (currHalls == 0b010)
                    dirsign++;
                else
                    dirsign--;
                break;

            case 0b010:
                if (currHalls == 0b011)
                    dirsign++;
                else
                    dirsign--;
                break;

            case 0b011:
                if (currHalls == 0b001)
                    dirsign++;
                else
                    dirsign--;
                break;

            case 0b001:
                if (currHalls == 0b101)
                    dirsign++;
                else
                    dirsign--;

                break;

            case 0b101:
                if (currHalls == 0b100)
                    dirsign++;
                else
                    dirsign--;

                break;

            case 0b100:
                if (currHalls == 0b110)
                    dirsign++;
                else
                    dirsign--;
                break;

            default:
                break;
        }

        if (dirsign > 3) {
            dirsign = 3;
            direction = FORWARD;
        } else {
            if (dirsign<-3) {
                dirsign = -3;
                direction = REVERSE;
            }
        }
        current_speed = MAV_filt((int16_t) (numerator / ticks));
        SPI2BUF = get_velocity(); //so that when the master reads it gets up to date velocity info
        ticks = 1;
        oldHalls = currHalls;

    } else {
        ticks++;
    }
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



    int hallA = HALLA;
    int hallB = HALLB;
    int hallC = HALLC;

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

    get_speed_and_direction(halls);
}

int16_t get_velocity(void) {
    int16_t velocity = current_speed;

    if (direction == REVERSE)
        velocity = -velocity;

    return velocity;
}

// this function looks at the current speed of the motor and the duty cycle command
//coming from the master. If the command would put more than 12 volts (but really
//11 for safety) accross the coil (back emf + vbat*duty cycle), the command is ignored
// and the function returns the current duty cycle and commanded direction.

int16_t abs_val(int16_t num) {
    if (num > 0) {
        return num;
    } else {
        return -num;
    }

}

int16_t safety_check(int16_t command) {
    int32_t command32 = (int32_t) (abs_val(command));
    int32_t curr_speed32 = (int32_t) current_speed;

    int32_t VmC;
    if (direction == FORWARD) {
        if (command >= 0) {
            VmC = VbattCmax * command32 - curr_speed32 * 10;
        } else {
            VmC = VbattCmax * command32 + curr_speed32 * 10;
        }
    } else {
        if (command < 0) {
            VmC = VbattCmax * command32 - curr_speed32 * 10;
        } else {
            VmC = VbattCmax * command32 + curr_speed32 * 10;
        }
    }


    //just for testing
    //    current_command = command;
    //    return command;

    // if it would put more that 11 volts on the motor, ignore the command
    if (VmC > VmCmax) {
        return current_command;
    } else {
        //command is accepted as safe and implemented
        current_command = command;
        return command;
    }
}

void update_duty_cycle_and_direction(int16_t command) {

    //ignore command if outside bounds
    if (command > 1000) {
        return;
    }
    if (command<-1000) {
        return;
    }
    command = safety_check(command); // perform a safety check to not burn motor

    if (command < 0) {
        duty_cycle = -command;
        drive_direction = REVERSE;
    } else {
        duty_cycle = command;
        drive_direction = FORWARD;
    }
}


// timer interrupts call the function which reads the halls and re-evaluates
// which coils to turn on

// the behavior is commanded by the master over SPI. If the current SPI command is 0x8000, this means
// no change, the master is reading the speed. A command between 100
uint32_t tt = 0;

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {



    block_com(duty_cycle, drive_direction);


    //following code used for testing the interrupt frequency
    // to make sure it's what we think it is (156,250 hz), we will make it count to
    // 60*156250 and if it takes a minute we'll know it's good

    //    tt++;
    //    if (tt > 8203125) {
    //        LED1 = 0;
    //    }



    TMR2 = 0x0000;
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
}


#ifdef TEST_COMMAND
/* In this test we set make the MDB think the motor is spinning at 6000 RPM forward
 * and then we send in commands via SPI. We send both allowed commands (between
 * +1000 and -1000, and not causing more than 11 v of drive voltage + back emf), and 
 * commands that do not satisfy this and will not be accepted. We send back the command
 * being set for execution to make sure that A) you can give the motor speed commands
 * over SPI and B) it will ignore the ones that we want it to ingore.
 
 */


#include "SPIcoms.h"

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
    //pwmInit();
    //hallsInit();
    //commutationInit();
    SPIinit();
    direction = REVERSE;
    current_speed = 4000;

    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    LED2 = 0;
    LED1 = 0;
    int16_t num16 = -30000;
    int32_t num32 = (int32_t) (abs(num16));

    if (num32 == 30000) {
        LED2 = 1;
    }

    //SPI_send(1);
    //SPI_send(2);
    //SPI_send(3);
    while (1);
}


#endif


#ifdef TEST_SPEED_READINGS
#include "SPIcoms.h"


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
    pwmInit();
    hallsInit();
    commutationInit();
    
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;

    LED2 = 1;
    LED1 = 0;
    while (1);
}
#endif

#ifdef test_clock_period
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

void test_timer_init() {
    T2CONbits.TON = 0; // Ensure Timer 2 is in reset
    T2CONbits.TCS = 0; // internal clock
    T2CONbits.TGATE = 0; // disable gated mode
    T2CONbits.TCKPS = 0b11; // prescaler 256
    TMR2 = 0; // clear timer register
    PR2 = 0x0001; // period

    IPC1bits.T2IP = 0X01; // set interrupt priority
    IFS0bits.T2IF = 0; // clear inteerupt flag
    IEC0bits.T2IE = 1; // enable inteerupt
    T2CONbits.TON = 1; // start timer


    // Use system clock Fosc/2 = 40 MHz. Result is a 156,250 Hz clock with a 1:256 prescaler. The clock is not
    // really very exact since we're using the internal RC oscillator. Testing shows that real clock period is closer
    //to 0.00000753371s or a frequency of 132736.725996 Hz, though this was not measured very scientifically.
}
uint32_t tt = 1;

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {

    //    the following code counts up to 156250 * 600 = 93750000 which should take the nominal clock
    // 10 minutes. Time it to see how long it really takes and that tells you the
    // clock period. // with timer 1, it takes: 7.something 6 3something, 6.33<-exact
    // with timer 2 // 12' 14" <-exact, (verified). This means a frequency of 127,725.

    // This test was performed with nothing but the timer running.
    // Result is a frequency of

    tt++;
    if (tt > 93750000) {
        LED1 = 0;
    }



    TMR2 = 0x0000;
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
}

int main(void) {
    clockInit();
    test_timer_init();
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    LED2 = 1;
    LED1 = 1;
    while (1);
}

#endif