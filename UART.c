#include "inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "UART.h"

void UART_init (void){

    SYSCTL_RCGCUART_R |= 0x04;          // activating UART2 from UART-RCGC
    SYSCTL_RCGCGPIO_R |= 0x08;          // enabling clock on port D
    GPIO_PORTD_AFSEL_R |= 0xC0;         // adjusting ASFEL register to enable alternate function on PD6,PD7
    GPIO_PORTD_PCTL_R |= 0x11000000 ;   // adjusting PCTL register to enable alternate function on PD6,PD7 see table in page 1344 and page 685

    UART2_CTL_R &= ~0x01; //disabling UART

    // adjusting the baud rate to 9600 --> default BR of the sensor
    // baud rate = clk freq / (16 * n)
    // if clk freq is 16 Mhz --> n ~ 104.1667

    UART2_IBRD_R = 0x68 ;  //integer value = 104
    UART2_FBRD_R = 0x0B ;  //value of 11 in fractional register as 11/64 = 0.17

        //////////////////////////////////////////////////////////////////////////////
    GPIO_PORTD_DEN_R |= 0xC0;       // digital enable on PD6,PD7
    UART2_LCRH_R |= 0x60;           // selecting 8-bit word length and enabling FIFO buffers and choosing only one stop bit
    UART2_CC_R = 0x0;               // use system clock
    UART2_CTL_R |= 0x301;           // enabling UART
}

char UART_In_char(void){
    while ((UART2_FR_R & 0x40) == 0 );     // wait for the receiver FIFO is Full
    return  ((char) (UART2_DR_R & 0xFF)) ;  // now return char type casted UART_data_Register value
}

void take_stream(void){
    for(i=0 ; i<650 ; i++)           // here is the loop which takes max 250 chars string from the user and input to the tiva
      {
        input_stream[i] = UART_In_char();   // take each char and assign it to the array
      }
}





