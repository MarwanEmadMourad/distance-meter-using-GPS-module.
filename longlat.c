#include "longlat.h"


void GPGLL_to_decimalDegrees( char input[] /* input from the sensor */ , char decimalDegreeslat[] /* output Decimal lat */, char decimalDegreeslong[] /* output Decimal long */ )
{
    
    char slat[] = "0000000000"; /* Exracted DMT lat */
    char slong[] = "0000000000"; /* Exracted DMT long */
    bool gpglat = false; /* DMT lat indicator */
    bool gpglong = false; /* DMT long indicator */
    bool gpgll = false; /* GPGLL reading indicator */
    int it_location;
    
    for (size_t i = 0; i < 250; i++)
    {
        if( i + 4 < 250 && input[i] == 'G' && input[ i + 1 ] == 'L' && input[ i + 2 ] == 'L' ) /* GPGLL reading */
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
                if (input[i] != ',') 
                {
                    slat[ i - it_location ] = input[i];
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
                if (input[i] != ',')
                {
                    slong[ i - it_location ] = input[i];
                }
                else
                {
                    gpglong = false;
                    break;
                }    
            }

        }
            
        
    }
    DMT_to_decimalDegrees( slat , slong , decimalDegreeslat , decimalDegreeslong); 

}

void DMT_to_decimalDegrees(char slat[] /* input lat DMT */, char slong[] /* input long DMT */, char decimalDegreeslat[] /* output Decimal lat */ , char decimalDegreeslong[] /* output Decimal long */)
{   
    
    char degreelat[] = "00"; /* Extracted lat Degree from DMT */
    char minlat[] = "00000000"; /* Extracted lat Minutes from DMT */
    char degreelong[] = "000"; /* Extracted long Degree from DMT */
    char minlong[] = "00000000"; /* Extracted long Minutes from DMT */
    float decimallat; /* Decimal Degrees lat */
    float decimallong; /* Decimal Degrees long */

    for (size_t i = 0; i < 10; i++)
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
    
    decimallat = strtof( degreelat , NULL ) + strtof( minlat , NULL ) / 60; /* Degrees + Minuties/60 */ 
    decimallong = strtof( degreelong , NULL ) + strtof( minlong , NULL ) / 60; /* Degrees + Minuties/60 */ 
    sprintf( decimalDegreeslat , "%f" , decimallat); /* float to String */
    sprintf( decimalDegreeslong , "%f" , decimallong); /* float to String */
    lat_long[0] = decimallat; /* Global float lat */
    lat_long[1] = decimallong; /* Global float long */
}


