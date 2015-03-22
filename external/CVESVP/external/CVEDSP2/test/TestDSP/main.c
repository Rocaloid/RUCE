#include "Interface/Setting.h"
#include "Container/InfWave.h"
#include "Container/Wave.h"
#include "Container/Spectrum.h"
#include <RFNL.h>

int main()
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    /*
    CDSP2_Wave_Double twave;
    CDSP2_Wave_Double_CtorSize(& twave, 1024);
    int i;
    for(i = 0; i < 1024; i ++)
        twave.Data[i] = i;
    
    Double* temp = RAlloc_Double(3000);
    for(i = 0; i < 3000; i ++)
        temp[i] = 0.2 * i;
    CDSP2_Wave_Double_Write(& twave, temp, - 500, 1000);
    CDSP2_Wave_Double_Read(& twave, temp, -1000, 3000);
    for(i = 0; i < 3000; i ++)
        printf("%f\n", temp[i]);
    RFree(temp);
    CDSP2_Wave_Double_Dtor(& twave);*/
    
    int i;
    // int l;
    CDSP2_InfWave_Double twave;
    CDSP2_InfWave_Double_CtorSize(& twave, 10000);
    CDSP2_InfWave_Double_Relocate(& twave, 15000);
    
    Double* temp = RAlloc_Double(10000);
    Double* temp2 = RAlloc_Double(6000);
    Double* wind = RAlloc_Double(1024);
    CDSP2_VSet_Double(wind, 1, 1024);
    CDSP2_Hanning_Double(wind, wind, 1024);
    CDSP2_IWave_Double_SetWindow(& twave, wind, 1024);
    
    for(i = 0; i < 3000; i ++)
        temp[i] = sin(0.2f * i);

    CDSP2_IWave_Double_Add(& twave, temp, 15500, 3000);
    CDSP2_InfWave_Double_Submit(& twave, 18500);
    CDSP2_IWave_Double_AddW(& twave, temp, 16500);
    CDSP2_IWave_Double_AddW(& twave, temp, 17000);
    
    CDSP2_IWave_Double_AddW(& twave, temp, 19000);
    CDSP2_InfWave_Double_Submit(& twave, 20500);
    
    RFNL_FWindow_Gnrc_Double fwin;
    RFNL_FWindow_Gnrc_Double_Ctor(& fwin);
    CDSP2_FWindow_Double_SetPara(& fwin, 21, 1080, 50);
    CDSP2_FWindow_Double_SetFunc(& fwin,
        RFNL_Blackman_Size_Gnrc_Double, RFNL_Blackman_Valu_Gnrc_Double);
    RFNL_FWindow_Gnrc_Double_Initialize(& fwin);
    
    CDSP2_Spectrum_Double spec;
    CDSP2_Spectrum_Double_CtorSize(& spec, 1024);
    spec.MagnType = CDSP2_dBMagn;
    
    CDSP2_Spectrum_Double_FromWaveW(& spec, & twave, 19000);
    CDSP2_VSet_Double(spec.Phse + 35, 0, 100);
    CDSP2_Spectrum_Double_ToReal(& spec, temp);
    CDSP2_InfWave_Double_Submit(& twave, 25000);
    
    //l = CDSP2_InfWave_Double_Dump(& twave, temp);
    //CDSP2_IWave_Double_FWRead(& twave, temp, & fwin, 22000, 500);
    for(i = 0; i < 1024; i ++)
        printf("%f\n", temp[i]);
    
/*
    double Real[2048];
    double Imag[2048];
    CDSP2_InfWave_Double_Read(& twave, temp2, 17000, 2048);
    CDSP2_RFFT_Double(Real, Imag, temp2, 11);
    CDSP2_IRFFT_Double(temp, Real, Imag, 11);
*/
    RFree(temp);
    RFree(temp2);
    RFree(wind);
    CDSP2_InfWave_Double_Dtor(& twave);
    RDelete(& spec, & fwin);
    return 0;
}

