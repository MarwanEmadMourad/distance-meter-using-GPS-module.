#include "Nokia5110.h"

enum typeOfWrite{
  COMMAND,                              // the transmission is an LCD command
  DATA                                  // the transmission is data
};

// static refers to that this function belongs to this file only
// not accessible by any other file.
void static lcdwrite(enum typeOfWrite type, char message){
  // if you are sending command
  if(type == COMMAND){
    // wait until SSI0 not busy/transmit FIFO empty
    while((SSI0_SR_R&0x00000010)==0x00000010){};
    DC = DC_COMMAND;
    SSI0_DR_R = message ;                // command out
    // wait until SSI0 not busy/transmit FIFO empty
    while((SSI0_SR_R&0x00000010)==0x00000010){};
  }

  // if you are sending data
  else{
    // wait until transmit FIFO not full
    while((SSI0_SR_R&0x00000002)==0){};
    DC = DC_DATA;
    SSI0_DR_R = message;                // data out

  }
}

void Nokia5110_Init(void){
  volatile unsigned long delay;
  SYSCTL_RCGC1_R |= 0x10;               // activate SSI0
  SYSCTL_RCGC2_R |= 0x01;               // activate port A
  delay = SYSCTL_RCGC2_R;               // allow time to finish activating
  GPIO_PORTA_DIR_R |= 0xC0;             // make PA6,7 out
  GPIO_PORTA_AFSEL_R |= 0x2C;           // enable alt funct on PA2,3,5 (PA2->SSI0Clk),(PA3->SSI0Fss),(PA5->SSI0Tx)
  GPIO_PORTA_AFSEL_R &= ~0xC0;          // disable alt funct on PA6,7
  GPIO_PORTA_DEN_R |= 0xEC;             // enable digital I/O on PA2,3,5,6,7
  GPIO_PORTA_PCTL_R |= 0x00202200;      // configure PA2,3,5 as SSI

  SSI0_CR1_R &= ~0x00000002;            // disable SSI
  SSI0_CR1_R &= ~0x00000004;            // master mode

  // configure your SSI clock source
  SSI0_CC_R = 0x00 ;                    // choosing 16Mhz system clock
  SSI0_CPSR_R = 10 ;                    // clk prescale divisor

  // Bitrate = SysClk / (CPSDVSR * (1 + SCR)) ;
  // (SysClk = 16MHz  , CPSDVSR = 10  ,  SRC = 0) --> BitRate = 1.6 Mb/sec.
  // Note that CPSDVSR must be an even value.

  SSI0_CR0_R &=~0xFF00 ;                // SRC value = 0
  SSI0_CR0_R |= 0x07 ;                  // 8-bit data
  SSI0_CR1_R |= 0x00000002;            // enable SSI

  // configuring LCD parameters
  RESET = RESET_LOW;                    // reset the LCD to a known state
  for(delay=0; delay<10; delay=delay+1);// delay minimum 100 ns
  RESET = RESET_HIGH;                   // negative logic

  lcdwrite(COMMAND, 0x21);              // chip active; horizontal addressing mode (V = 0); use extended instruction set (H = 1)

  lcdwrite(COMMAND, CONTRAST);          // set LCD (contrast)
  lcdwrite(COMMAND, 0x04);              // set temp coefficient
  lcdwrite(COMMAND, 0x14);              // LCD bias mode

  lcdwrite(COMMAND, 0x20);              // we must send 0x20 before modifying the display control mode
  lcdwrite(COMMAND, 0x0C);              // set display control to normal mode
}

void Nokia5110_OutChar(unsigned char data){
  int i;
  lcdwrite(DATA, 0x00);                 // blank vertical line padding
  for(i=0; i<5; i++){
    lcdwrite(DATA, ASCII[data - 0x20][i]);
  }
  lcdwrite(DATA, 0x00);                 // blank vertical line padding
}

void Nokia5110_OutString(char *ptr){
  while(*ptr){
    Nokia5110_OutChar((unsigned char)*ptr);
    ptr = ptr + 1;
  }
}

void Nokia5110_OutUDec(unsigned short n){
  if(n < 10){
    Nokia5110_OutString("    ");
    Nokia5110_OutChar(n+'0'); /* n is between 0 and 9 */
  } else if(n<100){
    Nokia5110_OutString("   ");
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  } else if(n<1000){
    Nokia5110_OutString("  ");
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
  else if(n<10000){
    Nokia5110_OutChar(' ');
    Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
  else {
    Nokia5110_OutChar(n/10000+'0'); /* ten-thousands digit */
    n = n%10000;
    Nokia5110_OutChar(n/1000+'0'); /* thousands digit */
    n = n%1000;
    Nokia5110_OutChar(n/100+'0'); /* hundreds digit */
    n = n%100;
    Nokia5110_OutChar(n/10+'0'); /* tens digit */
    Nokia5110_OutChar(n%10+'0'); /* ones digit */
  }
}


void Nokia5110_SetCursor(unsigned char newX, unsigned char newY){
    // available range for the LCD given the size of the screen and the size of each char
    if((newX > 11) || (newY > 5)){        // bad input
        return;
    }
  // multiply newX by 7 because each character is 7 columns wide
  lcdwrite(COMMAND, 0x80|(newX*7));     // setting bit 7 updates X-position
  lcdwrite(COMMAND, 0x40|newY);         // setting bit 6 updates Y-position
}


void Nokia5110_Clear(void){
  int i;
  for(i=0; i<(MAX_X*MAX_Y/8); i++){
    // just printing blank vertical lines (48*84)pixels/8(bits/line)
    lcdwrite(DATA, 0x00);
  }
  Nokia5110_SetCursor(0, 0);
}
