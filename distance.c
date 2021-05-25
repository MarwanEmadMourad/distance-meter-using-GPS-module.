#include <stdio.h>
#include <math.h>

float distance(const float long1,const float lat1 ,const float long2 ,const float lat2)
{
    float longrad1 = long1/(180/M_PI);
    float latrad1 = lat1/(180/M_PI);
    float longrad2 = long2/(180/M_PI);
    float latrad2 = lat2/(180/M_PI);

    float longrad_diff = longrad2 - longrad1;
    float latrad_diff = latrad2 - latrad1;

    float ans = sin( latrad_diff / 2 ) * sin( latrad_diff / 2 ) + cos(latrad1) * cos(latrad2) * sin( longrad_diff / 2 ) * sin( longrad_diff / 2 );
    
    ans = 2 * asin( sqrt( ans ) );
    unsigned int R = 6371*1000;
    ans = ans * R;
    return ans;
}
