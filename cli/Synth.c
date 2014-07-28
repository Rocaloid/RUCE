#include "Synth.h"

int SynthUnit(_Wave* Dest, _Wave* Sorc, RUCE_DB_Entry* SorcDB,
    CSVP_PitchModel* PM, RUCE_ResamplerPara* Para)
{
    int i;
    
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    
    //Initialize analysis/synthesis windows
    Real* Window = RCall(RAlloc, Real)(WinSize);
    RCall(CDSP2_GenHanning, Real)(Window, WinSize);
    
    _FWindow_T DyWin;
    RCall(_FWindow_T, Ctor)(& DyWin);
    RCall(_FWindow, SetPara)(& DyWin, 30, 3000, 20);
    RCall(_FWindow, SetFunc)(& DyWin, 
        RFNL_Hanning_Size_Gnrc_Float, RFNL_Hanning_Valu_Gnrc_Float);
    RCall(_FWindow, Initialize)(& DyWin);
    
    //Initialize waves
    _Wave ConWave, VowWave;
    RNew(_Wave, & ConWave, & VowWave);
    Real SampleRate = Sorc -> SampleRate;
    ConWave.SampleRate = SampleRate;
    VowWave.SampleRate = SampleRate;
    RCall(_Wave, Resize)(& ConWave, Sorc -> Size);
    RCall(_Wave, Resize)(& VowWave, Sorc -> Size);
    RCall(_Wave, SetWindow)(& ConWave, Window, WinSize);
    RCall(_Wave, SetWindow)(& VowWave, Window, WinSize);
    RCall(_Wave, SetWindow)(Sorc, Window, WinSize);
    
    //Initialize PSOLA-based consonant synthesis
    _PSOLAItersizer ConSynth;
    RCall(_PSOLAItersizer, Ctor)(& ConSynth);
    RCall(_PSOLAItersizer, SetWave)(& ConSynth, & ConWave);
    RCall(_PSOLAItersizer, SetPosition)(& ConSynth, 0);
    RCall(_PSOLAItersizer, SetWindow)(& ConSynth, & DyWin);
    
    //Source PSOLA frames
    _List_DataFrame ConList;
    CDSP2_List_Int  ConPulse;
    _PMatch         ConMatch;
    
    RCall(_List_DataFrame, Ctor)(& ConList);
    RCall(CDSP2_List_Int, Ctor)(& ConPulse);
    RCall(_PMatch, Ctor)(& ConMatch);
    
    for(i = 0; i <= SorcDB -> PulseList_Index; i ++)
        CDSP2_List_Int_Add(& ConPulse, SorcDB -> PulseList[i]);
    RCall(_List_DataFrame, FromWave)(& ConList, Sorc, & ConPulse);
    RCall(CDSP2_List_Int_ToPMatch, Real)(& ConPulse, & ConMatch);
    
    //Frequency remapping
    int VOTIndex;
    Real VOTTime = (Real)SorcDB -> VOT / SampleRate;
    Array_IncFind(VOTIndex, Real, Para -> Freq.X, VOTTime);
    if(VOTIndex > 0)
    {
        Array_RemoveRange(Real, Para -> Freq.X, 0, VOTIndex - 1);
        Array_RemoveRange(Real, Para -> Freq.Y, 0, VOTIndex - 1);
        Real ConFreq = SampleRate / (Real)(ConPulse.Frames[1] -
            ConPulse.Frames[0]);
        
        Real StableFreq = RCall(_PMatch, Query)(& Para -> Freq, VOTTime).Y;
        RCall(_PMatch, AddPair)(& Para -> Freq, VOTTime, StableFreq);
        if(VOTTime > 0.05)
        {
            RCall(_PMatch, AddPair)(& Para -> Freq, 0, ConFreq);
            RCall(_PMatch, AddPair)(& Para -> Freq, VOTTime - 0.02, ConFreq);
        }else
            RCall(_PMatch, AddPair)(& Para -> Freq, 0, ConFreq);
    }
    
    RCall(_PMatch, Print)(& Para -> Freq);
    
    //Accumulative reconstruction
    Real CurrPos = 0;
    while(CurrPos < SorcDB -> VOT + 3000)
    {
        _Transition Trans = RCall(_PMatch, Query)(& ConMatch, CurrPos);
        Real Freq = RCall(_PMatch, Query)(& Para -> Freq, CurrPos / SampleRate)
            .Y;
        RCall(_PSOLAItersizer, Add)(& ConSynth, & ConList.Frames
            [Trans.LowerIndex], CurrPos);
        CurrPos += SampleRate / Freq;
    }
    
    RCall(_PSOLAItersizer, IterNextTo)(& ConSynth, SorcDB -> VOT + 3000);
    RCall(_Wave, From)(Dest, & ConWave);
    
    RFree(Window);
    RDelete(& DyWin, & ConWave, & VowWave, & ConSynth, & ConList, & ConPulse,
        & ConMatch);
    
    return 1;
}

