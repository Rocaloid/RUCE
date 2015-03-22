#include <RUtil2.h>
#include <math.h>
#include "Vec/Vec.h"
#include "FMath/FMath.h"

void RFNL_Use();
int main()
{
    float* a, *b, *c, *d;
    RFNL_Use();
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
        
        RFNL_VAdd_Gnrc_Float(b, b, a, 1024);
        RFNL_VCplxAbs_Gnrc_Float(c, a, d, 1024);
        RFNL_VCplxMul_Gnrc_Float(d, c, d, c, b, a, 1024);
    }
    
    for(i = 0; i < 1024; i ++)
        printf("%.4d %.9f\n", i, d[i]);
    
    RFree(a, b, c, d);
    return 0;
}

