#include "F0.h"
#include "EndPoint.h"
#include <CVEDSP2.h>

int main()
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    
    CDSP2_Wave_Float MyWave;
    CDSP2_Wave_Float_Ctor(& MyWave);
    
    Float* wind = RAlloc_Float(2048);
    CDSP2_GenHanning_Float(wind, 2048);
    CDSP2_Wave_Float_SetWindow(& MyWave, wind, 2048);
    
    String_FromChars(Path, "/tmp/rec_part1.wav");
    
    CDSP2_Wave_Float_FromFile(& MyWave, & Path);
    
    CSVP_F0Iterlyzer_Float F0Iter;
    CSVP_F0Iterlyzer_Float_Ctor(& F0Iter);
    F0Iter.Option.Adlib = 1;
    F0Iter.Option.LFreq = 50;
    F0Iter.Option.HFreq = 700;
    F0Iter.Option.Threshold = 0.01;
    F0Iter.Option.YIN.W = 300;
    F0Iter.Option.YIN.Threshold = 0.2;
    F0Iter.Option.Method = CSVP_F0_YIN;
    
    int Onset = CSVP_OnsetFromWave_Float(& MyWave, 0.005, 0, MyWave.Size);
    
    CSVP_F0Iterlyzer_Float_SetHopSize(& F0Iter, 256);
    CSVP_F0Iterlyzer_Float_SetWave(& F0Iter, & MyWave);
    CSVP_F0Iterlyzer_Float_SetPosition(& F0Iter, Onset + 2000);
    CSVP_F0Iterlyzer_Float_PreAnalysisTo(& F0Iter, Onset + 10000);
    
    CSVP_F0Iterlyzer_Float_PrevTo(& F0Iter, 0);
    CSVP_F0Iterlyzer_Float_IterNextTo(& F0Iter, MyWave.Size - 3000);
    CSVP_F0PostProcess_Float(& F0Iter.F0List, 4000, 0.15, 100, 700);
    
    int i;
    for(i = 0; i <= F0Iter.F0List.X_Index; i ++)
        printf("%d, %f\n", (int)F0Iter.F0List.X[i], F0Iter.F0List.Y[i]);
    /*
    for(i = 0; i < MyWave.Size - 3000; i += 100)
        printf("%f\n", PMatch_Float_Float_Query(& F0Iter.F0List, i).Y);
    */
    RDelete(& MyWave, & Path, & F0Iter);
    RFree(wind);
    return 0;
}

