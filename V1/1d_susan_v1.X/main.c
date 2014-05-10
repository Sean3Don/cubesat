/*
 * File:   main.c
 * Author: Dmitriy
 *
 * Created on May 8, 2014, 2:21 PM
 */
#define RUN_MAIN

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
    init_all_getters();
    init_all_setters();
    run(10);
    
    return 0;
}
#endif