#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "Timer.h"

void Timer0A_init(){
        SYSCTL_RCGCTIMER_R |= 0x01 ;      // enabling clock on timer 0
        TIMER0_CTL_R = 0x0 ;              // disabling timer 0
        TIMER0_CFG_R = 0x0 ;              // clearing the timer configuration register
        TIMER0_TAMR_R = 0x2 ;             // setting the timer to be periodic timer
        TIMER0_TAILR_R = 0xFFFFFFFF ;     // setting the reload value to [value(in secs)]*[bus frequency]
        TIMER0_ICR_R |= 0x01;             // clear the time out flag
        TIMER0_IMR_R |= 0x01 ;            // enabling interrupts on timer A
        NVIC_PRI4_R = 0x0  ;              // setting the priority for the interrupt to be 0
        NVIC_EN0_R |= 0x00080000 ;        // Enabling NVIC for the interrupt no.(19) -> timer-0A (16/32-bits)
        TIMER0_CTL_R = 0x01 ;             // enabling timer 0
}



void Timer0A_Hnadler(){
    TIMER0_ICR_R |= 0x01 ;  // clearing the raw status bit means that i acknowledge the interrupt
    semaphore = 1 ;
}

void delay(float value){
    TIMER0_TAILR_R = (16000000-1)*value ;
    while (!semaphore);
    semaphore = 0 ;
}



