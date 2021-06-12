#include "longlat.h"

// This function slice out the long and lat vals from the GPS sensor
void GPGLL_to_decimalDegrees(char input_stream[] /* input from the sensor */ , char decimalDegreeslat[] /* output Decimal lat */, char decimalDegreeslong[] /* output Decimal long */ )
{
    static unsigned int j = 0 ;
    char slat[] = "0000000000"; /* Exracted DMT lat */
    char slong[] = "0000000000"; /* Exracted DMT long */
    bool gpglat = false; /* DMT lat indicator */
    bool gpglong = false; /* DMT long indicator */
    bool gpgll = false; /* GPGLL reading indicator */
    int it_location=0;

    for ( i = 0; i < 650; i++)
    {
        if( i + 4 < 650 && input_stream[i] == 'G' && input_stream[ i + 1 ] == 'L' && input_stream[ i + 2 ] == 'L' ) /* GPGLL reading */
        {
            i = i + 4; /* jump to lat */
            it_location = i; /* store lat start */
            gpgll = true;
            gpglat = true; 
        }
        if ( gpgll )
        {
            if (gpglat)
            {
                if (input_stream[i] != ',')
                {
                    slat[ i - it_location ] = input_stream[i];
                }
                else
                {
                    gpglat = false;
                    gpglong = true;
                    i = i + 3; /* jump to long */
                    it_location = i; /* store long start */
                }
            }
            if (gpglong)
            {
                if (input_stream[i] != ',')
                {
                    slong[ i - it_location ] = input_stream[i];
                }
                else
                {
                    gpglong = false;
                    break;
                }
            }
        }
    }
    // this part is responsible for saving the obtained coordinates in a char array "to_be_sent"
    if (j<1000)
        {
            int k,m ;
            for (m=0 ; m <2 ; m++,j++){
                for(k=0; k <10 ; k++){
                    if (m==0)
                        to_be_sent[j][k] = slat[k] ;
                    else
                        to_be_sent[j][k] = slong[k] ;
                }
            }
        }
    DMS_to_decimalDegrees( slat , slong , decimalDegreeslat , decimalDegreeslong);
}


// This function is responsible for converting the long and lat from degrees to decimal and to float data type
void DMS_to_decimalDegrees(char slat[] /*input lat DMT*/, char slong[] /* input long DMT */, char decimalDegreeslat[] /* output Decimal lat */ , char decimalDegreeslong[] /* output Decimal long */)
{   
    char degreelat[] = "00"; /* Extracted lat Degree from DMT */
    char minlat[] = "00000000"; /* Extracted lat Minutes from DMT */
    char degreelong[] = "000"; /* Extracted long Degree from DMT */
    char minlong[] = "00000000"; /* Extracted long Minutes from DMT */
    float decimallat; /* Decimal Degrees lat */
    float decimallong; /* Decimal Degrees long */

    for (i = 0; i < 10; i++)
    {
        if ( i < 2 ) /* first two number for Degrees */
        {
            degreelat[i] = slat[i];
        }
        else /* the rest for Minutes */
        {
            minlat[ i - 2 ] = slat[i];
        }
    
        if (i < 3) /* first three number for Degrees */
        {
            degreelong[i] = slong[i];
        }
        else /* the rest for Minutes */
        {
            minlong[ i - 3 ] = slong[i];
        }
    }
    decimallat = strtof( degreelat , NULL ) + (strtof( minlat , NULL ) / 60); /* Degrees + Minutes/60 */
    decimallong = strtof( degreelong , NULL ) + (strtof( minlong , NULL ) / 60); /* Degrees + Minutes/60 */
    lat_long[0] = decimallat; /* Global float lat */
    lat_long[1] = decimallong; /* Global float long */
}
