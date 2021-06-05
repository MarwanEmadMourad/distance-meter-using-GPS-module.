#include "To_PC.h"



void To_PC_init (void){

    (*((volatile uint32_t *)0x400FE618)) |= 0x01;  //activating UART0 from UART-RCGC
    (*((volatile uint32_t *)0x400FE608)) |= 0x01;  //enabling clock on port A
    (*((volatile uint32_t *)0x40004420)) |= 0x03;  //adjusting ASFEL register to enable alternate function on PA0,PA1
    (*((volatile uint32_t *)0x4000452C)) |= 0x11 ; //adjusting PCTL register to enable alternate function on PA0,PA1 see table in page 1344 and page 685


    (*((volatile uint32_t *)0x4000C030)) &= ~0x01; //disabling UART

         // adjusting the baud rate to 115200
         // baud rate = clk freq / (16 * n)
         // if clk freq is 16 Mhz --> n ~ 8.68

     (*((volatile uint32_t *)0x4000C024)) = 0x08 ;  //integer value = 8
     (*((volatile uint32_t *)0x4000C028)) = 0x2C ;  //value of 44 in fractional register as 44/64 = 0.68

         //////////////////////////////////////////////////////////////////////////////

     (*((volatile uint32_t *)0x4000451C)) |= 0x03;  //digital enable on PA0,PA1
     (*((volatile uint32_t *)0x4000C02C)) |= 0x60;  //selecting 8-bit word length and enabling FIFO buffers and choosing only one stop bit
     (*((volatile uint32_t *)0x4000CFC8)) = 0x0;    //use system clock

     (*((volatile uint32_t *)0x4000C030)) |= 0x301;  //enabling UART
}

void UART_Out_char(char data)
{
    while ( (UART0_FR_R & 0x80) == 0 );  // wait until the transmitter FIFO is empty

    UART0_DR_R = data ;     // now put the input data into UART_DATA_R in order for it to be sent

}

//void print (char * s) {
//    while(*s){
//        UART_Out_char(*s);
//        s++ ;
//    }
//}
void print_string(char s[],int n){
        /*this function aims to print any string from the tiva through the serial port to the PC*/
    int i=0 ;
    for (;i<n;i++){
        UART_Out_char(s[i]) ; // echoing one character at a time
    }
        UART_Out_char('\n');
        UART_Out_char('\r');
}
