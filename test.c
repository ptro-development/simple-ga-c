/* cos example */
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int main ()
{
    double param, result;
    param = 60.0;
    result = cos (param*PI/180);
    printf ("The cosine of %lf degrees is %lf.\n", param, result );
    return 0;
}
