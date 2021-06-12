#include <stdint.h>
#include <math.h>
#include <string.h>
#include "inc/tm4c123gh6pm.h"
#include "UART.h"
#include "longlat.h"
#include "distance.h"
#include "Nokia5110.h"
#include "To_PC.h"
#include "LED.h"
#include "Timer.h"


float FR [4] ;                       // array that holds 2 coordinate readings (lat1,long1 , lat2,long2)
float Total_distance = 0 ;           // global variable for the distance to be accumulated in
char td[5] ;                         // total distance in char data type to be sent to be plotted
uint32_t semaphore = 0 ;             // Timer Flag

int main(void)
    {
    Timer0A_init();  				         // timer init
    Nokia5110_Init();                        // LCD init
    Nokia5110_Clear() ;                      // Displaying The Word "Distance Covered"
    Nokia5110_SetCursor(0,1) ;
    Nokia5110_OutString("  Distance");
    Nokia5110_SetCursor(0,2) ;
    Nokia5110_OutString("  Covered:");
    UART_init();                            // GPS UART init
    To_PC_init();                           // Tiva to PC UART init
    PORTF_init();                           // Port F for the LED and the switch
    delay(3);
    take_stream();                          // Taking The first reading from the GPS sensor
    GPGLL_to_decimalDegrees(input_stream,decimalDegreeslat,decimalDegreeslong);
    FR[0] = lat_long[0];
    FR[1] = lat_long[1];

    while(1){
        // press on sw2 to start calculating distance
        if (!(GPIO_PORTF_DATA_R &= 0x01)){
            // calculating and displaying the distance loop
            while(1){
                delay(0.65);                // constant delay between each 2 readings, the delay value is based on the best outcome we got
                take_stream();
                GPGLL_to_decimalDegrees(input_stream,decimalDegreeslat,decimalDegreeslong);
                FR[2] = lat_long[0];
                FR[3] = lat_long[1];

                // Accumulating the distance
                Total_distance += distance(FR[0],FR[1],FR[2],FR[3]);
                Nokia5110_SetCursor(2,4) ;
                Nokia5110_OutUDec(Total_distance);  // Displaying the distance after each change

                // updating the second reading in the last sample to be the first in the new sample
                FR[0]=FR[2];
                FR[1]=FR[3];

                // Checking if the distance is above 100m to turn on the red LED
                if (Total_distance >100){
                    turn_on_red_led();
                    break ;
                }
            }

            // converting float distance to a char array to be sent to pc for displaying
            sprintf(td, "%2.13f", Total_distance);

            // loop responsible for sending the saved coordinates to draw the trajectory then sending the total distance covered
            while(1){
                int i ;
                for (i = 0 ; i < 1000 ; i++ ){
                    // if we found an empty string in the to_be_sent array
                    // send the word "end", refering to the end of the coordinates transferring
                    if(!strcmp(to_be_sent[i],""))
                    {
                        print_string("end\n\r",3);
                        break ;
                    }
                    print_string(to_be_sent[i],10) ;
                }
                // sending total distance covered
                print_string(td,5) ;
                UART_Out_char('\n') ;
                UART_Out_char('\r') ;
                delay(10);
                break ;
            }
        }
    }
}
