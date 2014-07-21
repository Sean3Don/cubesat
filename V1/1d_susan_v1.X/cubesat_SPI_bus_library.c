
#include <xc.h>

#include "serial.h"
#include "stdio.h"
#include "BOARD.h"
#include "cubesat_SPI_bus_library.h"
#include <peripheral/spi.h>


#define WAIT_TIME 1000
#define SS_AHRS PORTGbits.RG9
#define AHRS_START_TX 0xF6
#define AHRS_RX 0xFF

// THIS FUNCTION IS BLOCKING (but not for too long)

void blck_wait(int cycles) {
    while (cycles > 0) {
        cycles--;
    }
}

// use this to send one byte to the AHRS it toggles the SS pin low, waits a bit,
//sends the message, then toggles it high and waits a bit
// ONLY FOR AHRS

unsigned int txrx_byte(unsigned int byte) {
    unsigned int read;
    blck_wait(WAIT_TIME);
    SS_AHRS = 0;
    SpiChnPutC(2, byte);
    read = SpiChnGetC(2);
    //printf("r %x\n",read);
    SS_AHRS = 1;
    blck_wait(WAIT_TIME);
    return read;
}

void send_AHRS_command(unsigned int command) {

    SpiChnClose(SPI_CHANNEL2);
    unsigned int config = SPI_CON_MODE8 | SPI_CON_MSTEN | SPI_CON_CKE;
    // the last number is the clock divider
    SpiChnOpen(SPI_CHANNEL2, config, 16);

    TRISGbits.TRISG9 = 0;
    SS_AHRS = 1;

    //initiate coms
    printf("%d\n",txrx_byte(AHRS_START_TX));


    //put the command byte
    printf("%d\n",txrx_byte(command));

   //wait for AHRS to send back a 1, sending 0xFF over and over till it does
    unsigned int a = 10;

    while (a != 0x01) {
        a = txrx_byte(AHRS_RX);
       printf("rcvd %d\n", a);
    }
}

uint32_t * get_AHRS_data(unsigned int data_request, unsigned int byte_num) {

    //ask for some data
    send_AHRS_command(data_request);

    //create a receive array with length given by byte_num
    uint32_t receive_array[byte_num];

    int i = 0;
    for (i; i < byte_num; i++) {
        //for each byte, get txrx_byte sends back a unsigned int, then and with 0xff and cast to char
        //to keep the 8 lsbs
        receive_array[i] = txrx_byte(AHRS_RX);
    }

    SS_AHRS = 1;
}

#ifdef TEST_SPI

#define TIMER_VAL 0xFFFF

void timer_init(void) {
    T2CON = 0x0;
    TMR2 = 0x0;
    PR2 = TIMER_VAL;

    IPC2SET = 0x0000000D;
    IFS0CLR = 0x00000100;
    IEC0SET = 0x00000100;
    //turn on timer and set prescaler
    T2CONSET = 0x8070;
}

float angle = 0;

void __ISR(_TIMER_2_VECTOR, ipl3) Timer2Handler(void) {
    T2CONSET = 0x8000;


    uint32_t *receive_array;

    receive_array = get_AHRS_data(0x01,12);

    //read off the 12 bytes that come back as 3 floats
    float  angles[3];
    unpack_data(angles,receive_array,3);
    // now angles contains 3 euler angles, pitch, yaw, roll in that order
    printf("yaw %f\n", angles[1]);
    IFS0CLR = 0x00000100;

}

void main() {

    BOARD_Init();
    printf("Board inited, running main \n");

    int i = 0;
    while(i<10000000){
        i++;
    }

    send_AHRS_command(0x60);
    printf("SPI inited");
    timer_init();
    //printf("timers inited");
}

#endif

