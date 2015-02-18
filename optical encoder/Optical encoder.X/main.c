/* 
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on September 25, 2014, 1:41 PM
 */
#include <xc.h>
#include <stdio.h>
#include "BOARD.h"
#include "AD.h"
#include "serial.h"
/*
 * 
 */
void main() {
    BOARD_Init();
    SERIAL_Init();
    AD_Init();
    AD_AddPins(AD_PORTV5);
    printf("heyoh");
    printf("%d \n",AD_ReadADPin(AD_PORTV5));
    while(1){
        while(!AD_IsNewDataReady());
        printf("%d \n",AD_ReadADPin(AD_PORTV5));
        int i =0;
        for(i; i<1000000;i++);
    }

}

