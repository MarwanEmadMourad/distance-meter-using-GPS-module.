#ifndef TO_PC_H_
#define TO_PC_H_

#include "inc/tm4c123gh6pm.h"
#include <stdint.h>

void To_PC_init (void);
void UART_Out_char(char data);
void print_string(char *string,int n);


#endif
