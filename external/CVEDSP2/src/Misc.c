#include "Misc.h"

int CDSP2_IntLogOf2(int Num)
{
    int i = 0;
    while(Num > 1)
    {
        i ++;
        Num >>= 1;
    }
    return i;
}

