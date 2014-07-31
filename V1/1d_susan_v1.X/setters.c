/* File: setters.c
 * Author: Dmitriy Rivkin
 *
 *
 *
 *
 * Created May 8, 2014


 */
#include "xc.h"
#include "setters.h"
#include "stdint.h"
#include "stdio.h"
#include "RC_Servo.h"
#include "serial.h"
#include "BOARD.h"
#include "peripheral/spi.h"
#include "peripheral/timer.h"
//#include "peripheral/ports.h"


#define MDBSS PORTDbits.RD3 // should be RD3
#define WAIT_TIME 1000

void init_all_setters(void) {
    init_ESC_pulse();
}

void init_ESC_pulse(void) {
    // currently using RCpin x4 as defined by RC_Servo.h
    //look to IO_ports.h to figure out what pin this corresponds to on the micro
    //corresponds to B0 on the micro and J5 15 on the UNO32

    RC_Init();
    RC_AddPins(ESC_PIN);
    int i = 0;
    for (i; i < 1000000; i++);
    RC_SetPulseTime(ESC_PIN, 1500); //set intial speed to 0 rpm

}

void set_ESC_pulse(unsigned int pulse_width) {
    if (pulse_width > 2000)
        pulse_width = 2000;

    if (pulse_width < 1000)
        pulse_width = 1000;

    RC_SetPulseTime(ESC_PIN, pulse_width);
}

//this function blocks

void waitabit(int time) {
    int i = 0;
    while (i < time) {
        i++;
    }
}

void init_MDB(void) {
    TRISDbits.TRISD3 = 0;
    PORTDbits.RD3 = 1;
    MDBSS = 1;
    waitabit(1000);
    printf("done waiting\n");
    printf("done waiting\n");

}

int16_t motor_command(int16_t command) {
    unsigned int config = SPI_CON_MODE16 | SPI_CON_MSTEN | SPI_CON_CKE;
    // the last number is the clock divider
    SpiChnOpen(SPI_CHANNEL2, config, 256);
    MDBSS = 0;
    waitabit(WAIT_TIME);
    SpiChnPutC(2, command);
    int16_t velocity = SpiChnGetC(2);
    MDBSS = 1;
    return velocity;
}

#ifdef CALIBRATE_ESC
//this routine is used for calibrating the ESC, you need to do it if it beeps at you
//but doesnt spin

void main() {
    BOARD_Init();
    init_ESC_pulse();
    printf("max pulse \n");
    set_ESC_pulse(2000);
    GetChar();
    while (IsReceiveEmpty()) {
    }
    printf("min pulse \n");
    set_ESC_pulse(1000);
    GetChar();
    while (IsReceiveEmpty()) {
    }
    printf("off pulse \n");
    set_ESC_pulse(1500);

}

#endif

#ifdef ESC_TEST

int main(void) {
    BOARD_Init();
    init_ESC_pulse();
    unsigned int i = 1700;
    for (i; i > 1300; i = i - 10) {
        printf("%d\n", i);
        set_ESC_pulse(i);
        GetChar();
        while (IsReceiveEmpty()) {
        }
    }

    return 0;
}
#endif

#ifdef MDB_test


#include "AHRS.h"
#include "getters.h"
#define TIMER_VAL 0xffff

typedef union {
    int16_t i16;
    uint16_t ui16;
} conversion_union;

void timer_init(void) {
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, TIMER_VAL);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_3);
    INTEnableSystemMultiVectoredInt();
}

void __ISR(_TIMER_3_VECTOR, ipl3) Timer3Handler(void) {
    mT3ClearIntFlag();
    printf("timeout\n");
    //printf("mspeed %d\n\n",motor_command(-300));
    //waitabit(1000);
    printf("angle %f\n",get_AHRS_angle());
    waitabit(1000);
    printf("mspeed %d\n\n",motor_command(-300));
    
    //MDBSS ^= 1;
}

int main(void) {
    BOARD_Init();

    printf("board_inited\n");
    init_MDB();
   
    AHRS_init();
    printf("ahrs inited\n");
     timer_init();

     while(1);

//    int i = 800;
//    while (i > -900) {
//        while (IsReceiveEmpty());
//        printf("looped\n");
//        printf("current angle: %f\n",get_AHRS_angle());
//        waitabit(1000);
//        printf("current speed: %d\n", motor_command(-i));
//        printf("sent command %d\n\n", -i);
//        GetChar();
//        i = i - 100;
//    }
}


#endif