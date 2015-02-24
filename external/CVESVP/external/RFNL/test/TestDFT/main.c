#include <RUtil2.h>
#include <math.h>
#include "DFT/DFT.h"

int main()
{
    Float* Real, *Imag;
    Float* DReal, *DImag;
    Float* Temp;
    Real = RAlloc_Float(1024);
    Imag = RAlloc_Float(1024);
    Temp = RAlloc_Float(1024);
    DReal = RAlloc_Float(1024);
    DImag = RAlloc_Float(1024);
    int i;
    for(i = 0; i < 1024; i ++)
    {
        Real[i] = sin(i * 0.03f);
        Temp[i] = Real[i];
        Imag[i] = 0;
    }
    RFNL_RFFT_Gnrc_Float(DReal, DImag, Real, 10);
    RFNL_FFTReflect_Gnrc_Float(DReal, DImag, 10);
    for(i = 0; i < 10000; i ++)
        RFNL_IRFFT_Gnrc_Float(Real, DReal, DImag, 10);
    for(i = 0; i < 1024; i ++)
        printf("%.9f\n", Real[i] - Temp[i]);
    //    printf("%f\n", sqrt(DReal[i] * DReal[i] + DImag[i] * DImag[i]));
    RFree(Real);
    RFree(Imag);
    RFree(DReal);
    RFree(DImag);
    RFree(Temp);
    return 0;
}

