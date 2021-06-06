#include "LED.h"


void PORTF_init(void){

    *((volatile uint32_t*)0x400FE608) |= 0x00000020 ;  // initializing clock to port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;                    // unlock GPIO Port F
    GPIO_PORTF_CR_R |= 0x1F;                           // allow changes to PF4-0
    *((volatile uint32_t*)0x40025400) |= 0x00000002 ;  // initializing red led as o/p
    *((volatile uint32_t*)0x4002551C) |= 0x00000003 ;  // activating digital enable on PF0,PF1-->(red led, SW2)
    *((volatile uint32_t*)0x40025420) &= 0x00 ;        // disable alternative functions
    *((volatile uint32_t*)0x4002552C) &= 0x00000000;   // also disabling alternative functions
    *((volatile uint32_t*)0x40025528) &= 0x00 ;        // disabling any analog input or output

}

void turn_on_red_led(){
    GPIO_PORTF_DATA_R |= 0x02 ; // turn on PF1--> red LED
}

void turn_off_red_led(){
    GPIO_PORTF_DATA_R &= ~ 0x02 ; // turn off red led
}

LED.c
1 KB