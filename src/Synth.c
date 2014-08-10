#include "Synth.h"
/*
static void HNMFrameAndPhaseFromWave(_HNMFrame* Dest, _DataFrame* DestPhse,
    _Wave* Sorc, Real F0, int Position)
{
    int WinSize = Sorc -> WinSize;
    _Spectrum Spec1, Spec2;
    RCall(_Spectrum, CtorSize)(& Spec1, WinSize);
    RCall(_Spectrum, CtorSize)(& Spec2, WinSize);
    
    RCall(_Spectrum, FromWaveW)(& Spec1, Sorc, Position - WinSize / 2);
    RCall(_Spectrum, FromWaveW)(& Spec2, Sorc, Position - WinSize / 2 + 1);
    
    Real NewF0 = RCall(CSVP_F0FromSuccSpectrum_Peak, Real)(& Spec1, & Spec2, 1,
        F0 * 0.5, F0 * 1.7);
    
    if(NewF0 < 0) NewF0 = F0;
    int HNum = 8000 / NewF0;
    RCall(_HNMFrame, Resize)(Dest, WinSize, HNum);
    RCall(_HNMFrame, FromSpectrumWithPhase)(Dest, DestPhse, & Spec1, NewF0,
        HNum);
    
    RDelete(& Spec1, & Spec2);
}
*/
static void InterpFetchHNMFrame(_HNMFrame* Dest, _List_HNMFrame* Sorc,
    _Transition* Trans)
{
    int LIndex, HIndex;
    LIndex = Trans -> LowerIndex;
    HIndex = Trans -> LowerIndex == Sorc -> Frames_Index ? LIndex : LIndex + 1;
    _HNMFrame* LHNM = & Sorc -> Frames[LIndex];
    _HNMFrame* HHNM = & Sorc -> Frames[HIndex];
    
    RCall(_HNMFrame, Resize)(Dest, LHNM -> Size, LHNM -> Hmnc.Size);
    RCall(_Sinusoid, InterpFrom)(& Dest -> Hmnc, & LHNM -> Hmnc, & HHNM -> Hmnc,
        Trans -> X);
    
    RCall(CDSP2_VSet,  Real)(Dest -> Noiz, 0, Dest -> Size / 2 + 1);
    RCall(CDSP2_VFCMA, Real)(Dest -> Noiz, LHNM -> Noiz, 1.0 - Trans -> X,
        Dest -> Size / 2 + 1);
    RCall(CDSP2_VFCMA, Real)(Dest -> Noiz, HHNM -> Noiz, Trans -> X,
        Dest -> Size / 2 + 1);
    RCall(_HNMFrame, From)(Dest, LHNM);
}

int RUCE_SynthUnit(_Wave* Dest, _Wave* Sorc, RUCE_DB_Entry* SorcDB,
    CSVP_PitchModel* PM, RUCE_ResamplerPara* Para)
{
    int i, j;
    
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    
    //Initialize analysis/synthesis windows
    Verbose("Initializing windows...\n");
    Real* Window = RCall(RAlloc, Real)(WINSIZE);
    RCall(CDSP2_GenHanning, Real)(Window, WINSIZE);
    
    _FWindow_T DyWin;
    RCall(_FWindow_T, Ctor)(& DyWin);
    RCall(_FWindow, SetPara)(& DyWin, 30, 3000, 20);
    RCall(_FWindow, SetFunc)(& DyWin, 
        RFNL_Hanning_Size_Gnrc_Float, RFNL_Hanning_Valu_Gnrc_Float);
    RCall(_FWindow, Initialize)(& DyWin);
    
    //Initialize waves
    Verbose("Initializing waves...\n");
    _Wave ConWave, VowWave, NozWave;
    RNew(_Wave, & ConWave, & VowWave, & NozWave);
    Real SampleRate = Sorc -> SampleRate;
    ConWave.SampleRate = SampleRate;
    VowWave.SampleRate = SampleRate;
    NozWave.SampleRate = SampleRate;
    int DestSize = Para -> LenRequire * SampleRate;
    RCall(_Wave, Resize)(& ConWave, DestSize);
    RCall(_Wave, Resize)(& VowWave, DestSize);
    RCall(_Wave, Resize)(& NozWave, DestSize);
    RCall(_Wave, Resize)(Dest, DestSize);
    RCall(_Wave, SetWindow)(& ConWave, Window, WINSIZE);
    RCall(_Wave, SetWindow)(& VowWave, Window, WINSIZE);
    RCall(_Wave, SetWindow)(& NozWave, Window, WINSIZE);
    RCall(_Wave, SetWindow)(Sorc, Window, WINSIZE);
    
    //Source HNM frames
    Verbose("Initializing HNM frames...\n");
    int FrameNum = SorcDB -> FrameList_Index + 1;
    _List_HNMFrame  VowList;
    _List_DataFrame PhseList;
    _List_Int       VowPulse;
    _PMatch         VowMatch;
    
    RCall(_List_HNMFrame, CtorSize)(& VowList, FrameNum);
    RCall(_List_DataFrame, CtorSize)(& PhseList, FrameNum);
    RCall(_List_Int, CtorSize)(& VowPulse, FrameNum);
    RCall(_PMatch, Ctor)(& VowMatch);
    
    for(i = 0; i < FrameNum; i ++)
    {
        int HNum;
        
        RUCE_DB_Frame* SorcFrame = & SorcDB -> FrameList[i];
        _HNMFrame* DestFrame = & VowList.Frames[i];
        VowPulse.Frames[i] = SorcFrame -> Position;
        HNum = SorcFrame -> Freq_Index + 1;
        
        RCall(_HNMFrame, Resize)(DestFrame, WINSIZE, HNum);
        RCall(_DataFrame, Resize)(& PhseList.Frames[i], HNum);
        
        for(j = 0; j < HNum; j ++)
        {
            DestFrame -> Hmnc.Freq[j]  = SorcFrame -> Freq[j];
            DestFrame -> Hmnc.Ampl[j]  = SorcFrame -> Ampl[j];
            PhseList.Frames[i].Data[j] = SorcFrame -> Phse[j];
        }
        
        RCall(CDSP2_VSet, Real)(DestFrame -> Noiz, -999, WINSIZE / 2 + 1);
        RCall(CDSP2_Resample_Linear, Real)(DestFrame -> Noiz, SorcFrame -> Noiz,
            WINSIZE / 2 + 1, SorcDB -> NoizSize);
    }
    VowPulse.Frames_Index = FrameNum - 1;
    RCall(CDSP2_List_Int_ToPMatch, Real)(& VowPulse, & VowMatch);
    
    
    //Aperiodic component separation
    Verbose("Aperiodic component separation...\n");
    _SinusoidIterlyzer SAna;
    RCall(_SinusoidIterlyzer, Ctor)(& SAna);
    SAna.GenPhase = 1;
    SAna.LeftBound = SorcDB -> VOT + 1500;
    
    _PMatch F0List;
    RCall(_PMatch, Ctor)(& F0List);
    RCall(_List_HNMFrame, HToPMatch)(& VowList, & F0List, & VowPulse, 0);
    
    RCall(_SinusoidIterlyzer, SetHopSize)(& SAna, 128);
    RCall(_SinusoidIterlyzer, SetWave)(& SAna, Sorc);
    RCall(_SinusoidIterlyzer, SetPosition)(& SAna, SAna.LeftBound);
    RCall(_SinusoidIterlyzer, SetUpperFreq)(& SAna, 9000);
    RCall(_SinusoidIterlyzer, SetPitch)(& SAna, & F0List);
    
    RCall(_SinusoidIterlyzer, PrevTo)(& SAna, 0);
    RCall(_SinusoidIterlyzer, IterNextTo)(& SAna, SAna.LeftBound + 2000);
    
    RCall(CSVP_NoiseTurbFromSinuList, Real)(& ConWave, Sorc,
        & SAna.PulseList, & SAna.SinuList, & SAna.PhseList);
    RCall(_Wave, Resize)(& ConWave, VowWave.Size);
    
    //Time scale
    //X: Dest -> Y: Sorc
    Verbose("Time mapping for HNM composition...\n");
    _PMatch TimeMatch;
    RCall(_PMatch, Ctor)(& TimeMatch);
    RCall(_PMatch, AddPair)(& TimeMatch, 0, 0);
    RCall(_PMatch, AddPair)(& TimeMatch, SorcDB -> VOT, SorcDB -> VOT);
    
    if(Dest -> Size > SorcDB -> WaveSize - SorcDB -> InvarRight
        + SorcDB -> InvarLeft)
    {
        RCall(_PMatch, AddPair)(& TimeMatch, SorcDB -> InvarLeft,
            SorcDB -> InvarLeft);
        RCall(_PMatch, AddPair)(& TimeMatch, Dest -> Size - (SorcDB -> WaveSize
            - SorcDB -> InvarRight), SorcDB -> InvarRight);
    }else
        RCall(_PMatch, AddPair)(& TimeMatch, SorcDB -> InvarLeft,
            (SorcDB -> WaveSize + SorcDB -> VOT) / 2);
    
    RCall(_PMatch, AddPair)(& TimeMatch, Dest -> Size, SorcDB -> WaveSize);
    
    //Interpolate target HNM frames
    Verbose("Interpolating & pitch-scaling target HNM frames...\n");
    _HNMItersizer VowSynth;
    RCall(_HNMItersizer, CtorSize)(& VowSynth, WINSIZE);
    RCall(_HNMItersizer, SetHopSize)(& VowSynth, SorcDB -> HopSize);
    RCall(_HNMItersizer, SetWave)(& VowSynth, & VowWave);
    RCall(_HNMItersizer, SetNoizWave)(& VowSynth, & NozWave);
    
    _HNMFrame TempHNM;
    _HNMContour TempCont;
    RCall(_HNMFrame, Ctor)(& TempHNM);
    RCall(_HNMContour, Ctor)(& TempCont);
    int Position = VowPulse.Frames[0];
    int Count = 1;
    
    Para -> FlagPara.Gender = Para -> FlagPara.Gender < -99.0 ? -99.0 :
        Para -> FlagPara.Gender;
    Para -> FlagPara.Gender = Para -> FlagPara.Gender >  99.0 ?  99.0 :
        Para -> FlagPara.Gender;
                
    while(Position < Dest -> Size - SorcDB -> HopSize)
    {
        //TODO: Add linear interpolation
        int SorcPosition = (int)RCall(_PMatch, Query)(& TimeMatch, Position).Y;
        _Transition Trans = RCall(_PMatch, Query)(& VowMatch, SorcPosition);
        Real F0 = RCall(_PMatch, Query)(& Para -> Freq, (Real)Position
            / SampleRate).Y;
        
        InterpFetchHNMFrame(& TempHNM, & VowList, & Trans);
        
        CSVP_PitchConvertHNMFrame_Float(& TempCont, & TempHNM, PM, F0, 10000,
            SampleRate);
        
        if(Para -> FlagPara.Breathness != 50.0)
            RCall(CDSP2_VCAdd, Real)(TempCont.Noiz, TempCont.Noiz,
                log(Para -> FlagPara.Breathness / 50.0),
                WINSIZE / 2 + 1);
        
        if(Para -> FlagPara.Gender != 0)
        {
            _HNMContour NewCont;
            RCall(_HNMContour, CtorSize)(& NewCont, WINSIZE);
            
            Real GenderCoef = (100.0 - Para -> FlagPara.Gender) / 100.0;
            Real SorcAnchor[2], DestAnchor[2];
            
            SorcAnchor[0] = 0;
            DestAnchor[0] = 0;
            if(GenderCoef > 1)
            {
                DestAnchor[1] = WINSIZE / 2 + 1;
                SorcAnchor[1] = DestAnchor[1] / GenderCoef;
            }else
            {
                SorcAnchor[1] = WINSIZE / 2 + 1;
                DestAnchor[1] = SorcAnchor[1] * GenderCoef;
            }
            RCall(CDSP2_VSet, Real)(NewCont.Hmnc, -999, WINSIZE / 2 + 1);
            RCall(CDSP2_VSet, Real)(NewCont.Noiz, -999, WINSIZE / 2 + 1);
            
            _MapStretch(NewCont.Hmnc, TempCont.Hmnc, DestAnchor, SorcAnchor,
                2, WINSIZE / 2 + 1);
            _MapStretch(NewCont.Noiz, TempCont.Noiz, DestAnchor, SorcAnchor,
                2, WINSIZE / 2 + 1);
            
            RCall(_HNMContour, From)(& TempCont, & NewCont);
            RCall(_HNMContour, Dtor)(& NewCont);
        }
        
        RCall(_HNMFrame, FromContour)(& TempHNM, & TempCont, F0, 8000);
        RCall(_HNMItersizer, Add)(& VowSynth, & TempHNM, Position);
        
        if(Count % 10 == 0)
        {
            CSVP_PhaseSyncH_Float(& PhseList.Frames[Trans.LowerIndex], 0);
            CSVP_PhaseContract_Float(& PhseList.Frames[Trans.LowerIndex],
                CSVP_PitchModel_GetPhseCoh(PM, F0) *
                Para -> FlagPara.PhaseSync);
            RCall(_HNMItersizer, AddPhase)(& VowSynth, & PhseList.Frames
                [Trans.LowerIndex], Position);
        }
        Position += SorcDB -> HopSize;
        Count ++;
    }
    RCall(_HNMFrame, Dtor)(& TempHNM);
    RCall(_HNMContour, Dtor)(& TempCont);
    
    //HNM synthesis
    Verbose("HNM synthesis...\n");
    VowSynth.Option.PhaseControl = 1;
    int FirstPos  = VowSynth.PulseList.Frames[0];
    int ConcatPos = SorcDB -> VOT - 1500 < FirstPos ?
                    FirstPos : SorcDB -> VOT - 1500;
    int PhseIndex = CDSP2_List_Int_IndexAfter(& VowSynth.PulseList, ConcatPos);
    RCall(_HNMItersizer, SetPosition)(& VowSynth, ConcatPos);
    RCall(_HNMItersizer, SetInitPhase)(& VowSynth,
        & PhseList.Frames[PhseIndex]);
    RCall(_HNMItersizer, IterNextTo)(& VowSynth,
        TopOf(VowSynth.PulseList.Frames));
    if(ConcatPos > FirstPos)
    RCall(_HNMItersizer, PrevTo)(& VowSynth, FirstPos);
    
    //Concatenation
    Verbose("Concatenating...\n");
    int ConcatLen = 3000;
    for(i = 0; i < ConcatLen; i ++)
    {
        ConWave.Data[ConcatPos + i] *= 1.0 - (Real)i / ConcatLen;
        NozWave.Data[ConcatPos + i] *= (Real)i / ConcatLen;
    }
    RCall(CDSP2_VSet, Real)(NozWave.Data, 0, ConcatPos);
    RCall(CDSP2_VSet, Real)(ConWave.Data + i + ConcatPos, 0,
        ConWave.Size - i - ConcatPos);
    
    RCall(_Wave, From)(Dest, & NozWave);
    RCall(CDSP2_VAdd, Real)(Dest -> Data, Dest -> Data, ConWave.Data,
        Dest -> Size);
    RCall(CDSP2_VAdd, Real)(Dest -> Data, Dest -> Data, VowWave.Data,
        Dest -> Size);
    RCall(CDSP2_VCMul, Real)(Dest -> Data, Dest -> Data, Para -> Volume / 100.0,
        Dest -> Size);
    
    Verbose("Freeing memory...\n");
    RFree(Window);
    RCall(_SinusoidIterlyzer, Dtor)(& SAna);
    RCall(_PMatch, Dtor)(& F0List);
    RDelete(& DyWin, & ConWave, & VowWave, & NozWave,
        & VowList, & PhseList, & VowPulse, & VowMatch, & TimeMatch,
        & VowSynth);
    
    return 1;
}

