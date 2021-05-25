#ifndef LONGLAT_H_
#define LONGLAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char decimalDegreeslat[10];
char decimalDegreeslong[10];
float lat_long[2];
void DMS_to_decimalDegrees(char slat[] /* input lat DMS */, char slong[] /* input long DMS */, char decimalDegreeslat[] /* output Decimal lat */ , char decimalDegreeslong[] /* output Decimal long */);

void GPGLL_to_decimalDegrees( char input[] /* input from the sensor */ , char decimalDegreeslat[] /* output Decimal lat */, char decimalDegreeslong[] /* output Decimal long */ );


#endif
