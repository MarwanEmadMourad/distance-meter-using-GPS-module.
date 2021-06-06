#ifndef UART_H_
#define UART_H_

char input_stream[650] ;
int i  ;

void UART_init (void);
char UART_In_char(void);
void take_stream(void);
//void UART_Out_char(char data);
//void print_string(char *string);


#endif
