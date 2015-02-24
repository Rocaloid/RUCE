#include "PSOLA.h"
#include "F0.h"
#include "EndPoint.h"
#include <CVEDSP2.h>

#define Wave CDSP2_Wave_Float
#define InfWave CDSP2_InfWave_Float
#define IWave CDSP2_IWave_Float
#define PSOLAIterlyzer CSVP_PSOLAIterlyzer_Float
#define PSOLAItersizer CSVP_PSOLAItersizer_Float
#define F0Iterlyzer CSVP_F0Iterlyzer_Float
#define List_DataFrame CDSP2_List_DataFrame_Float
#define List_Int CDSP2_List_Int
#define FWindow_T RFNL_FWindow_Gnrc_Float
#define FWindow CDSP2_FWindow_Float

int main(int argc, char** arg)
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    
    String Path;
    Wave InWave, OutWave;
    float* HannWind = RAlloc_Float(2048);
    
    RNew(Wave, & InWave, & OutWave);
    RNew(String, & Path);
    CDSP2_GenHanning_Float(HannWind, 2048);
    
    RCall(Wave, Resize)(& OutWave, 100000);
    RCall(Wave, SetWindow)(& InWave, HannWind, 2048);
    RCall(Wave, SetWindow)(& OutWave, HannWind, 2048);
    
    #ifdef __WIN32__
    String_SetChars(& Path, "C:\\xxx.wav");
    #else
    String_SetChars(& Path, arg[1]);
    #endif
    RCall(Wave, FromFile)(& InWave, & Path);
    
    int VOT = CSVP_VOTFromWave_Float(& InWave, 0, InWave.Size / 2);
    int Onset = CSVP_OnsetFromWave_Float(& InWave, 0.0005, 0, InWave.Size);
    
    CSVP_F0Iterlyzer_Float F0Iter;
    CSVP_F0Iterlyzer_Float_Ctor(& F0Iter);
    
    CSVP_F0Iterlyzer_Float_SetHopSize(& F0Iter, 256);
    CSVP_F0Iterlyzer_Float_SetWave(& F0Iter, & InWave);
    CSVP_F0Iterlyzer_Float_SetPosition(& F0Iter, VOT + 2000);
    CSVP_F0Iterlyzer_Float_PreAnalysisTo(& F0Iter, VOT + 10000);
    
    CSVP_F0Iterlyzer_Float_IterNextTo(& F0Iter, InWave.Size - 1000);
    CSVP_F0Iterlyzer_Float_PrevTo(& F0Iter, 0);
    
    PSOLAIterlyzer PAna;
    RNew(PSOLAIterlyzer, & PAna);
    RCall(PSOLAIterlyzer, SetWave)(& PAna, & InWave);
    RCall(PSOLAIterlyzer, SetPosition)(& PAna, VOT + 2000);
    RCall(PSOLAIterlyzer, SetBound)(& PAna, VOT);
    RCall(PSOLAIterlyzer, SetPitch)(& PAna, & F0Iter.F0List);
    if(! RCall(PSOLAIterlyzer, PreAnalysisTo)(& PAna, VOT + 6000))
    {
        printf("Preanalysis failed.\n");
        return 1;
    }
    
    RCall(PSOLAIterlyzer, IterNextTo)(& PAna, InWave.Size);
    
    if(! RCall(PSOLAIterlyzer, PrevTo)(& PAna, Onset))
    {
        printf("Backward analysis failed.\n");
        //return 1;
    }
    printf("%f\n", F0Iter.RefF0);
    
    
    int i;
    
    for(i = 0; i <= PAna.PulseList.Frames_Index; i ++)
    {
        int p = RCall(PSOLAIterlyzer, Fetch)(& PAna, i);
        printf("%f %f\n", (float)p / InWave.SampleRate
                        , (float)p / InWave.SampleRate);
    }
    
    
    List_DataFrame PSOLAFrame;
    RCall(List_DataFrame, Ctor)(& PSOLAFrame);
    RCall(List_DataFrame, FromWave)(& PSOLAFrame, & InWave, & PAna.PulseList);
    
    PSOLAItersizer PSyn;
    RCall(PSOLAItersizer, Ctor)(& PSyn);
    RCall(List_DataFrame, From)(& PSyn.DataList, & PSOLAFrame);
    RCall(List_Int, From)(& PSyn.PulseList, & PAna.PulseList);
    
    
    for(i = 1; i <= PSyn.PulseList.Frames_Index; i ++)
    {
        PSyn.PulseList.Frames[i] = PSyn.PulseList.Frames[i - 1] + 
            (PAna.PulseList.Frames[i] - PAna.PulseList.Frames[i - 1]) * 0.5;
    }
    
    FWindow_T DyWin;
    RCall(FWindow_T, Ctor)(& DyWin);
    RCall(FWindow, SetPara)(& DyWin, 30, 3000, 20);
    RCall(FWindow, SetFunc)(& DyWin, 
        RFNL_Hanning_Size_Gnrc_Float, RFNL_Hanning_Valu_Gnrc_Float);
    RCall(FWindow, Initialize)(& DyWin);
    
    OutWave.SampleRate = InWave.SampleRate;
    RCall(PSOLAItersizer, SetWave)(& PSyn, & OutWave);
    RCall(PSOLAItersizer, SetWindow)(& PSyn, & DyWin);
    RCall(PSOLAItersizer, SetPosition)(& PSyn, VOT);
    RCall(PSOLAItersizer, RepositionFrom)(& PSyn, 0);
    
    RCall(PSOLAItersizer, PrevTo)(& PSyn, 0);
    RCall(PSOLAItersizer, IterNextTo)(& PSyn, InWave.Size);
    #ifdef __WIN32__
    String_SetChars(& Path, "C:\\out.wav");
    #else
    String_SetChars(& Path, "/tmp/out.wav");
    #endif
    RCall(Wave, ToFile)(& OutWave, & Path);
    
    RFree(HannWind);
    //RDelete(& InWave, & OutWave, & Path);
    RDelete(& InWave, & OutWave, & Path, & PAna, & PSyn, & PSOLAFrame, & DyWin,
        & F0Iter);
    return 0;
}

