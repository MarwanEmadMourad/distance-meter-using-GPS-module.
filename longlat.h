#ifndef LONGLAT_H_
#define LONGLAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int i ;
char decimalDegreeslat[10];
char decimalDegreeslong[10];
double lat_long[2];
extern char input_stream[];     // buffer to hold the input stream from the GPS sensor
char to_be_sent[1000][10] ;     // an array of size 1000 needed to hold the coordinates values as we walk to send it to the PC to be plotted

void GPGLL_to_decimalDegrees( char input_stream[] /* input from the sensor */ , char decimalDegreeslat[] /* output Decimal lat */, char decimalDegreeslong[] /* output Decimal long */ );
void DMS_to_decimalDegrees(char slat[] /* input lat DMT */, char slong[] /* input long DMT */, char decimalDegreeslat[] /* output Decimal lat */ , char decimalDegreeslong[] /* output Decimal long */);


#endif
