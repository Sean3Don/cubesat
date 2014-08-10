
#define WAIT_TIME 100
#define SS PORTGbits.RG9
#define START_TX 0xF6
#define RX 0xFF
#define TARE 0x60
#define TARED_EULER_ANGLES 0x01
#define NORM_GYRO_RATE 0x21



#include "xc.h"
#include "serial.h"
#include "stdio.h"
#include "BOARD.h"
#include "getters.h"
#include "AHRS.h"
#include <peripheral/spi.h>


// union used for data unpacking
typedef union {
	float r32;
	uint32_t u32;
} conv_union;


// THIS FUNCTION IS BLOCKING (but not for too long)
void blck_wait(int cycles) {
    while (cycles > 0) {
        cycles--;
    }
}

// use this to send one byte to the AHRS it toggles the SS pin low, waits a bit,
//sends the message, then toggles it high and waits a bit
unsigned int txrx_byte(unsigned int byte){
    unsigned int read;
    blck_wait(WAIT_TIME);
    SS=0;
    SpiChnPutC(2,byte);
    read=SpiChnGetC(2);
    //printf("r %x\n",read);
    SS=1;
    blck_wait(WAIT_TIME);
    return read;
}

/*
 * unpack_data is passed a pointer to an empty array of floats and a full array
 * of uint32_t that contain the bytes of  data received from the AHRS. The compiler
 * is little endian and the data is received big endian.

 */
void unpack_data(float* floats, uint32_t* bytes, int float_array_size){

    int i = 0;
    int j;
    conv_union tmp;

    for(i;i<float_array_size;i++){
        j=i*4;
        tmp.u32 = bytes[j+3] | (bytes[j+2] << 8) | (bytes[j+1] << 16) | (bytes[j] << 24);
        //printf("%x\n",tmp.u32);
        floats[i]=tmp.r32;
        //printf("%f\n",floats[i]);
    }
}

/* Send command is passed a command to send. It sends it and then waits for the
 * AHRS to send back a 1, meaning that the next read byte read will either be a
 * zero (if the command was not one that receives data in the anser), or the beginning
 * of the data sent back by the AHRS.
 *

 */
void send_command(unsigned int command){

    unsigned int config = SPI_CON_MODE8 | SPI_CON_MSTEN|SPI_CON_CKE ;
    // the last number is the clock divider
    SpiChnOpen(SPI_CHANNEL2, config, 256); //256 works, //4 doesnt
    SS = 1;
    //initiate coms
    txrx_byte(START_TX);

    //put the command byte

    txrx_byte(command);

    //wait for AHRS to send back a 1, sending 0xFF over and over till it does
    unsigned int a=txrx_byte(RX);
    while(a != 0x01){
        a=txrx_byte(RX);
    }
}

void AHRS_init(void) {
    //wait for the AHRS to wake up
    blck_wait(10000000);


    // this part sets up the SPI interface\
    // channel 2 is the one for J8 on the UNO32
    // new data is placed on line at falling edge
//    unsigned int config = SPI_CON_MODE8 | SPI_CON_MSTEN|SPI_CON_CKE ;
//    // the last number is the clock divider
//    SpiChnOpen(SPI_CHANNEL2, config, );


    // this is the slave select pin. it's controlled in software. you need to bring
    // it high when you're not transmitting and low 10ns before you start transmitting.
    // you also need to cycle it between characters
    TRISGbits.TRISG9 = 0;
    SS = 1;

    //the following does the tare (at the current position)
    send_command(TARE);

    //clear overflow bit
    SpiChnGetRov(2,TRUE);
}

float AHRS_get_yaw(void){
    //ask for some euler angles
    send_command(TARED_EULER_ANGLES);

    //create and receive array. 12 unsigned ints, but only the lowest byte
    //is used to create 3 floats.
    uint32_t receive_array[12];

    int i=0;
    for(i;i<12;i++){
        //for each byte, get txrx_byte sends back a unsigned int, then and with 0xff and cast to char
        //to keep the 8 lsbs
        receive_array[i]=txrx_byte(RX);
    }


    //read off the 12 bytes that come back as 3 floats
    float  angles[3];
    unpack_data(angles,receive_array,3);
    // now angles contains 3 euler angles, pitch, yaw, roll in that order

    //close the com channel
    SS=1;
    SpiChnClose(SPI_CHANNEL2);

    return angles[1];
}

float AHRS_get_yaw_rate(void){
    send_command(NORM_GYRO_RATE);

        //create and receive array. 12 unsigned ints, but only the lowest byte
    //is used to create 3 floats.
    uint32_t receive_array[12];

    int i=0;
    for(i;i<12;i++){
        //for each byte, get txrx_byte sends back a unsigned int, then and with 0xff and cast to char
        //to keep the 8 lsbs
        receive_array[i]=txrx_byte(RX);
    }


    //read off the 12 bytes that come back as 3 floats
    float  rates[3];
    unpack_data(rates,receive_array,3);
    // now angles contains 3 euler angles, pitch, yaw, roll in that order

    //close the com channel
    SS=1;
    SpiChnClose(SPI_CHANNEL2);

    return rates[1];

}

#ifdef AHRS_TEST

#define TIMER_VAL 0xFFFF

#include "peripheral/timer.h"

void timer_init(void) {
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, 0xffff);
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_3);
    INTEnableSystemMultiVectoredInt();
}

void __ISR(_TIMER_3_VECTOR, ipl3) Timer3Handler(void) {
    mT3ClearIntFlag();
    printf("yaw: %f\n",AHRS_get_yaw());
    printf("yaw rate: %f \n\n", AHRS_get_yaw_rate());

}





void main() {
    BOARD_Init();
    printf("starting\n");
    AHRS_init();
    printf("AHRS inited\n");
    timer_init();

    printf("%f\n",0x0fffffff);
    printf("%f\n",0xffffffff);
}

#endif