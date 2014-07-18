#include "xc.h"
#include "stdio.h"
#include <peripheral/uart.h>
#include "BOARD.h"

//#define RADIO_TEST

#ifdef RADIO_TEST
int main(void) {
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_NOT_FULL | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, BOARD_GetPBClock(), 115200);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));
    while (1) {
        while(!UARTReceivedDataIsAvailable(UART1));
        UARTGetDataByte(UART1);
        if (UARTTransmitterIsReady(UART1)) {
            UARTSendDataByte(UART1, 'a');
        }
    }
}

#endif
