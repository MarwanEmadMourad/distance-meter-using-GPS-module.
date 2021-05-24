#include "TM4C123.h"
#include <stdint.h>
#include <stdlib.h>

void Delay(unsigned long counter); // used to add delay
void Bluetooth_Write(unsigned char data); // Transmit a character to HC-05 over Tx5 pin 
