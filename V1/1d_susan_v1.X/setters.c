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
#include "RC_Servo.h"
#include "serial.h"
#include "BOARD.h"


void init_all_setters(void){
    init_ESC_pulse();
}

void init_ESC_pulse(void){
// currently using RCpin x4 as defined by RC_Servo.h
//look to IO_ports.h to figure out what pin this corresponds to on the micro
//corresponds to B0 on the micro and J5 15 on the UNO32

    RC_Init();
    RC_AddPins(ESC_PIN);
    int i = 0;
    for(i; i<1000000 ; i++ );
    RC_SetPulseTime(ESC_PIN,1500); //set intial speed to 0 rpm
    
}

void set_ESC_pulse(unsigned int pulse_width){
    if (pulse_width>2000)
        pulse_width=2000;

    if(pulse_width<1000)
        pulse_width=1000;

   RC_SetPulseTime(ESC_PIN,pulse_width);
}


#ifdef CALIBRATE_ESC
//this routine is used for calibrating the ESC, you need to do it if it beeps at you
//but doesnt spin

void main(){
    BOARD_Init();
    init_ESC_pulse();
    printf("max pulse \n");
    set_ESC_pulse(2000);
    GetChar();
    while(IsReceiveEmpty()){
    }
    printf("min pulse \n");
    set_ESC_pulse(1000);
    GetChar();
    while(IsReceiveEmpty()){
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

