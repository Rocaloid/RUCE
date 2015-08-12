        #define _ISOC9X_SOURCE  1
        #define _ISOC99_SOURCE  1
        #define __USE_ISOC99    1
        #define __USE_ISOC9X    1
        #include <math.h>
        int main (void)
        {       double  fval ;
                int k, ival ;

                fval = 1.0 * 0x7FFFFFFF ;
                for (k = 0 ; k < 100 ; k++)
                {       ival = (lrint (fval)) >> 24 ;
                        if (ival != 127)
                                return 1 ;

                        fval *= 1.2499999 ;
                        } ;

                        return 0 ;
                }
        }
