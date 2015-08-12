        #define _ISOC9X_SOURCE  1
        #define _ISOC99_SOURCE  1
        #define __USE_ISOC99    1
        #define __USE_ISOC9X    1
        #include <math.h>
        int main (void)
        {       double  fval ;
                int k, ival ;

                fval = -8.0 * 0x10000000 ;
                for (k = 0 ; k < 100 ; k++)
                {       ival = (lrint (fval)) >> 24 ;
                        if (ival != -128)
                                return 1 ;

                        fval *= 1.2499999 ;
                        } ;

                        return 0 ;
                }
        }
