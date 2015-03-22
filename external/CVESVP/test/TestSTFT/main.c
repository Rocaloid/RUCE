#include "STFT.h"
#include <CVEDSP2.h>

int main()
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);    
    
    CDSP2_Wave_Float MyWave, NewWave;
    CDSP2_Wave_Float_Ctor(& MyWave);
    CDSP2_Wave_Float_CtorSize(& NewWave, 48000 * 20);
    
    Float* wind = RAlloc_Float(2048);
    CDSP2_GenHanning_Float(wind, 2048);
    CDSP2_Wave_Float_SetWindow(& MyWave, wind, 2048);
    CDSP2_Wave_Float_SetWindow(& NewWave, wind, 2048);
    
    String_FromChars(Path, "/tmp/a.wav");
    CDSP2_Wave_Float_FromFile(& MyWave, & Path);
    
    //**********
    CSVP_STFTIterlyzer_Float DyAna;
    CSVP_STFTIterlyzer_Float_CtorSize(& DyAna, 2048);
    CSVP_STFTIterlyzer_Float_SetHopSize(& DyAna, 512);
    CSVP_STFTIterlyzer_Float_SetWave(& DyAna, & MyWave);
    CSVP_STFTIterlyzer_Float_SetPosition(& DyAna, 100000);
    
    CSVP_STFTIterlyzer_Float_IterNextTo(& DyAna, 120000);
    
    CSVP_STFTItersizer_Float DySyn;
    CSVP_STFTItersizer_Float_CtorSize(& DySyn, 2048);
    CSVP_STFTItersizer_Float_SetHopSize(& DySyn, 512);
    CSVP_STFTItersizer_Float_SetWave(& DySyn, & NewWave);
    CSVP_STFTItersizer_Float_SetPosition(& DySyn, 100000);
    CSVP_STFTItersizer_Float_ExtractFrom(& DySyn, & DyAna);
    CSVP_STFTItersizer_Float_IterNextTo(& DySyn, 300000);
    
    int i;
    for(i = 150000; i < 500000; i += 30000)
    {
        CSVP_STFTIterlyzer_Float_IterNextTo(& DyAna, i);
        CSVP_STFTItersizer_Float_ExtractFrom(& DySyn, & DyAna);
    }
    for(i = 0; i <= DySyn.PulseList.Frames_Index; i ++)
    {
        int j;
        for(j = 80; j < 150; j ++)
            DySyn.SpecList.Frames[i].Magn[j] = 0;
    }
    CSVP_STFTItersizer_Float_IterPrevTo(& DySyn, 0);
    
    RDelete(& DyAna, & DySyn);
    //**********
    
    String_SetChars(& Path, "/tmp/test3.wav");
    CDSP2_Wave_Float_ToFile(& NewWave, & Path);
    
    RDelete(& MyWave, & NewWave, & Path);
    RFree(wind);
    return 0;
}

