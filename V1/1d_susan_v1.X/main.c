/*
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on May 8, 2014, 2:21 PM
 */
#define RUN_MAIN
//#define TEST_BATTERY_LOCKOUT

#include "xc.h"
#include "stdio.h"
#include "BOARD.h"
#include "serial.h"
#include "setters.h"
#include "runner.h"
#include "Control.h"


#ifdef RUN_MAIN

int main(void) { 
     //do initialization

    BOARD_Init();
    printf("board inited\n");
    
    init_all_getters();
    init_all_setters();
    init_control();
    //while(!IsReceiveEmpty()); // start it remotely
    run(10);
    
    
    return 0;
}
#endif

#ifdef TEST_BATTERY_LOCKOUT

#include "AD.h"
#include "peripheral/timer.h"
void timer_init(void) {
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, 0xffff);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_3);
    INTEnableSystemMultiVectoredInt();
}

void __ISR(_TIMER_3_VECTOR, ipl3) Timer3Handler(void) {
    printf("voltage reading %d\n",AD_ReadADPin(BAT_VOLTAGE));
    mT3ClearIntFlag();

}
int main(void){
    BOARD_Init();
    AD_Init();
    AD_AddPins(BAT_VOLTAGE);
    timer_init();
    while(1);
}


#endif