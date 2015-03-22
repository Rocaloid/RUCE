#include <RUtil2.h>
#include "Vec/Vec.h"

int main()
{
    float* a, *b, *c, *d;
    a = RAlloc_Float(1024);
    b = RAlloc_Float(1024);
    c = RAlloc_Float(1024);
    d = RAlloc_Float(1024);
    
    int i;
    for(i = 0; i < 1024; i ++)
        a[i] = 0.1f * i;
    
    //This is non-sense...
    for(i = 0; i < 100000; i ++)
    {
        RFNL_VCopy_Gnrc_Float(b, a, 1024);
        RFNL_VCopy_Gnrc_Float(d, a, 1024);
        /*
        int j;
        for(j = 0; j < 1024; j ++)
            b[j] += a[j];
        */
        RFNL_VAdd_Gnrc_Float(b, b, a, 1024);
        RFNL_VCplxAbs_Gnrc_Float(c, a, d, 1024);
        RFNL_VCplxMul_Gnrc_Float(d, c, d, c, b, a, 1024);
    }
    
    for(i = 0; i < 1024; i ++)
        printf("%.4d %.9f\n", i, d[i]);
    
    RFree(a, b, c, d);
    return 0;
}

