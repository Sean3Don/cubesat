#include <xc.h>
#include <string.h>
#include <stdint.h>
#include "stdio.h"
#include <stdlib.h>
#include <uart.h>
#include "adc.h"
#include "math.h"
#include "commutation_SALT.h"

#define LED1   LATDbits.LATD11
#define LED2   LATDbits.LATD10
#define LED3   LATBbits.LATB12
#define LED4   LATBbits.LATB13

#define RESET3 LATDbits.LATD2
#define RESET2 LATDbits.LATD1
#define RESET1 LATDbits.LATD0

#define PWMINPUT PORTGbits.RG0
#define DIRINPUT PORTGbits.RG1

#define DISC_THRESH 2000 //Data points which are this far from the average will not be included in the average. Good for
// keeping clean numbers, but make sure it's faster than the acceleration of the motor, otherwise
// the speed gets stuck. This should be adjusted when the reaction wheel is added since the motor
// wont be able to accelerate as fast.

// the extra factor of 10 is to keep some precision for the duty cycle
#define VmCmax 58080 // Vmax = 11V VmCmax = 10*11V*(1000 mv/V) /(1.895 mv/rpm)
#define VbattCmax 63  // Vbatt=12V VbattCmax = 12V*(1000 mv/V)/((1.895 mv/rpm)*1000) the last factor of 1000 is because duty cycle goes up to 1000

#define SPEED_HYST_THRESH 100 //hysteresis threshold on speed (TC only)

#define HALLA  PORTFbits.RF0
#define HALLB   PORTFbits.RF1
#define HALLC  PORTDbits.RD9
#define FORWARD 1
#define REVERSE 0
#define UP 1
#define DOWN 0


// torque command
int32_t commanded_torque = 0;

//commanded (either directly or by torque controller):
int16_t duty_cycle = 0;
int16_t drive_direction = 1;
int16_t current_command = 0; // this is what is currently being executed

//measured:
int16_t direction = 1;
int16_t current_speed = 0;
uint32_t ticks = 1;

int32_t c1 = 16; //100000 *.001895(ke)/12 (Vbatt)
int32_t c2 = 13; // 100000*28.8(R)/(18100(Km in uNm/A)*12(Vbatt))

int max_speed_reached = 0;

int32_t numerator = 494934; // (60 s/m *127725 ticks/s)/(12 halls/rotation)*(6200/8000 correction factor)
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
    TRISFbits.TRISF0 = 1;

    // hall b
    TRISDbits.TRISD9 = 1;

    // hall c
    TRISFbits.TRISF1 = 1;

    int hallA = HALLA;
    int hallB = HALLB;
    int hallC = HALLC;

    char halls = 0x00;
    halls = hallA << 2 | hallB << 1 | hallC;

    oldHalls = halls;
}

void pwmInit(void) {
    PTCONbits.PTEN = 0;

    /* Setup for the Auxiliary clock to use the FRC as the REFCLK */
    /* ((FRC * 16) / APSTSCLR) = (7.49 * 16) / 1 = 119.84 MHz */
    ACLKCONbits.FRCSEL = 1; /* FRC is input to Auxiliary PLL */
    ACLKCONbits.SELACLK = 1; /* Auxiliary Oscillator provides the clock
	source */
    ACLKCONbits.APSTSCLR = 1; /* Divide Auxiliary clock by 1 */
    ACLKCONbits.ENAPLL = 1; /* Enable Auxiliary PLL */
    while (ACLKCONbits.APLLCK != 1); /* Wait for Auxiliary PLL to Lock */

    //setup PWM ports
    PWMCON1bits.ITB = 0; /* PTPER provides the PWM time period value */
    PWMCON2bits.ITB = 0; /* PTPER provides the PWM time period value */
    PWMCON3bits.ITB = 0; /* PTPER provides the PWM time period value */


    IOCON1bits.PENH = 1;
    IOCON2bits.PENH = 1;
    IOCON3bits.PENH = 1;

    //set the low side IO pins to be software controlled. Setting one
    // of these low causes the coil to go to high impedance
    IOCON1bits.PENL = 0;
    IOCON2bits.PENL = 0;
    IOCON3bits.PENL = 0;

    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD0 = 0;

    //Setup desired frequency by setting period for 1:1 prescaler
    PTPER = 1000;
    PHASE1 = 0;
    PHASE2 = 0;
    PHASE3 = 0;





    /* Set Duty Cycles */
    PDC1 = 500;

    PDC2 = 500;

    PDC3 = 500;


    RESET1 = RESET2 = RESET3 = 1;
    /* Set Dead Time Values */
    DTR1 = DTR2 = DTR3 = 0;
    ALTDTR1 = ALTDTR2 = ALTDTR3 = 0;
    PWMCON1 = PWMCON2 = PWMCON3 = 0x0000;
    /* Configure Faults */
    FCLCON1 = FCLCON2 = FCLCON3 = 0x0003;
    /* 1:1 Prescaler */
    PTCON2 = 0x0000;

    PTCONbits.PTEN = 1;
}

int16_t get_current_command(void) {
    return current_command;
}

int16_t abs_val(int16_t num) {
    if (num > 0) {
        return num;
    } else {
        return -num;
    }

}

void check_bounds(void) {
    if (max_speed_reached) {
        if (abs_val(current_speed) < MAX_SPEED - SPEED_HYST_THRESH) {
            max_speed_reached = 0;
            return;
        }
    } else {
        if (abs_val(current_speed) > MAX_SPEED + SPEED_HYST_THRESH) {
            max_speed_reached = 1;
        }
    }
}
//This is where torque control is implemented

void torque_control(void) {
    check_bounds();
    if (max_speed_reached) {
        duty_cycle = DUTY_CYCLE_MAX_SPEED;
        LED1 = 1;
    } else {
        int32_t speed = current_speed;
        if (direction == REVERSE) {
            speed = -speed;
        }
        int32_t kV = speed * c1 + commanded_torque*c2;
        update_duty_cycle_and_direction(kV / 100);
    }
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
        //LED2 ^= 1;
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
        //        if (direction) {
        //            LED1 = 1;
        //        } else {
        //            LED1 = 0;
        //        }
        current_speed = MAV_filt((int16_t) (numerator / ticks));
        SPI2BUF = get_velocity(); //so that when the master reads it gets up to date velocity info
        ticks = 1;
        oldHalls = currHalls;

    } else {
        ticks++;
    }
}

void PWM_update_command(int32_t up, int32_t down) {

    if (down == 0) {

        if (DIRINPUT == FORWARD) {
            update_duty_cycle_and_direction(1000);
        } else {
            update_duty_cycle_and_direction(-1000);
        }
    } else {

        int16_t com = (int16_t) (((up * 1000) / (up + down))); // multiply by 1000 then divide by total
        if (DIRINPUT == FORWARD) {

            update_duty_cycle_and_direction(com);


        } else {
            update_duty_cycle_and_direction(-com);
        }
    }
}


//
int32_t pwmInUp = 0; // pwm input uptime
int32_t pwmInDown = 0; // pwm input downtime
int16_t curState = DOWN;

void check_pwm_input(void) {

    if (curState == UP) {
        if (PWMINPUT == UP) {

            pwmInUp++;

        } else {

            pwmInDown++;
            curState = DOWN;
        }
    } else {
        if (PWMINPUT == UP) {

            PWM_update_command(pwmInUp, pwmInDown);
            pwmInUp = 1;
            pwmInDown = 0;
            curState = UP;
        } else {

            pwmInDown++;
        }
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



    int hallA = HALLA;
    int hallB = HALLB;
    int hallC = HALLC;

//    if(hallA){
//        LED2=1;
//    }else{
//        LED2 = 0;
//    }
//    if(hallB){
//        LED3 = 1;
//    }else{
//        LED3 = 0;
//    }
//    if(hallC){
//        LED4 = 1;
//    }else{
//        LED4 = 0;
//    }

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
            LED2=1;
            r1 = 0;

            dc3 = 0;

            break;

        case 0b010:
            LED3=1;
            r3 = 0;
            dc1 = 0;
            break;

        case 0b011:
            //LED4=1;
            r2 = 0;
            dc1 = 0;
            break;

        case 0b001:
            LED2=0;
            r1 = 0;
            dc2 = 0;
            break;

        case 0b101:
            LED3=0;
            r3 = 0;
            dc2 = 0;
            break;

        case 0b100:
            //LED4 = 0;
            r2 = 0;
            dc3 = 0;
            break;

        default:
            LED4 ^= 1;
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

    //get_speed_and_direction(halls);
    check_pwm_input();
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
    if (command > 400) {
        return;
    }
    if (command<-400) {
        return;
    }
    //command = safety_check(command); // perform a safety check to not burn motor

    if (command < 0) {
        duty_cycle = -command;
        drive_direction = REVERSE;
    } else {
        duty_cycle = command;
        drive_direction = FORWARD;
    }
}

void update_torque_command(int32_t tcommand) {
    // ignore command if outside of bounds
    if (tcommand > MAX_TORQUE) {
        return;
    }
    if (tcommand<-MAX_TORQUE) {
        return;
    }
    commanded_torque = tcommand;
}


// timer interrupts call the function which reads the halls and re-evaluates
// which coils to turn on

// the behavior is commanded by the master over SPI. If the current SPI command is 0x8000, this means
// no change, the master is reading the speed. A command between 100
uint32_t tt = 0;

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {



    block_com(duty_cycle, drive_direction);

#ifdef TORQUE_COMMAND
    if (tt == 5000) {
        tt++;
        torque_control();
        tt = 0;
    }
#endif


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


#ifdef PWM_COMS


_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
_FICD(JTAGEN_OFF & ICS_PGD1);

void ClockInit(void) {
#if defined(__dsPIC33FJ64GS608__)
    PLLFBD = 242; // M = 50 MIPS
    CLKDIVbits.PLLPOST = 0;
    CLKDIVbits.PLLPRE = 7;
    OSCTUN = 0;
    RCONbits.SWDTEN = 0;
    __builtin_write_OSCCONH(0x01); // Initiate Clock Switch to Primary (3?)
    __builtin_write_OSCCONL(0x01); // Start clock switching
    while (OSCCONbits.COSC != 0b001); // Wait for Clock switch to occur
    while (OSCCONbits.LOCK != 1) {
    };
#endif
}

//set up the pwm input from uno and direction line as inputs

void pwm_communications_init() {
    TRISBbits.TRISB7 = 1; //PWM
    TRISBbits.TRISB9 = 1; // direction
}

int main(void) {
    ClockInit();
    pwmInit();
    hallsInit();
    commutationInit();

    TRISDbits.TRISD11 = 0;
    TRISDbits.TRISD10 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;

    //update_duty_cycle_and_direction(500);
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    
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

#ifdef TEST_TORQUE_COMMAND
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
    pwmInit();
    hallsInit();
    commutationInit();
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    LED2 = 0;
    LED1 = 0;
    //update_duty_cycle_and_direction(100);
    //update_torque_command(0);
    while (1);
}




#endif
