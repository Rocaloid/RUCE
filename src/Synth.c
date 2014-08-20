#include "Synth.h"

static void InterpHNMFrame(_HNMFrame* Dest, _HNMFrame* Sorc1, _HNMFrame* Sorc2,
    Real Ratio)
{
    RCall(_HNMFrame, Resize)(Dest, Sorc1 -> Size, Sorc1 -> Hmnc.Size);
    RCall(_Sinusoid, InterpFrom)(& Dest -> Hmnc,
        & Sorc1 -> Hmnc, & Sorc2 -> Hmnc, Ratio);
    
    RCall(CDSP2_VSet,  Real)(Dest -> Noiz, 0, Dest -> Size / 2 + 1);
    RCall(CDSP2_VFCMA, Real)(Dest -> Noiz, Sorc1 -> Noiz, 1.0 - Ratio,
        Dest -> Size / 2 + 1);
    RCall(CDSP2_VFCMA, Real)(Dest -> Noiz, Sorc2 -> Noiz, Ratio,
        Dest -> Size / 2 + 1);
}

static void InterpFetchHNMFrame(_HNMFrame* Dest, _List_HNMFrame* Sorc,
    _Transition* Trans)
{
    int LIndex, HIndex;
    LIndex = Trans -> LowerIndex;
    HIndex = Trans -> LowerIndex == Sorc -> Frames_Index ? LIndex : LIndex + 1;
    _HNMFrame* LHNM = & Sorc -> Frames[LIndex];
    _HNMFrame* HHNM = & Sorc -> Frames[HIndex];
    
    InterpHNMFrame(Dest, LHNM, HHNM, Trans -> X);
}

typedef struct
{
    Real P1, P2, P3;
} Segmentation;

static Segmentation Resegment(Segmentation Sorc, Real NP3, Real DP1, Real DP2)
{
    Segmentation Ret;
    
    Real P1, P2;
    P1 = Sorc.P1;
    P2 = NP3 - (Sorc.P3 - Sorc.P2);
    
    if(P2 < P1)
        P1 = P2 = Sorc.P1 / (Sorc.P1 + Sorc.P2) * NP3;
    
    P1 += DP1;
    P2 -= DP2;
    
    if(P2 < P1)
    {
        P1 = P2 = (P2 + P1) / 2.0;
        if(P1 < 0) P1 = P2 = 0;
    }
    
    Ret.P1 = P1;
    Ret.P2 = P2;
    Ret.P3 = NP3;
    return Ret;
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
    int SorcSize = TopOf(SorcDB -> FrameList).Position;
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
    
    
    Verbose("Initializing aperiodic component of %d samples...\n",
        Sorc -> Size);
    /*
        Unnecessary part.
        We someday may need this F0 curve.
    */
    _PMatch F0List;
    RCall(_PMatch, Ctor)(& F0List);
    RCall(_List_HNMFrame, HToPMatch)(& VowList, & F0List, & VowPulse, 0);
    
    RCall(_Wave, From)(& ConWave, Sorc);
    RCall(_Wave, Resize)(& ConWave, VowWave.Size);
    
    //Time scale
    //X: Dest -> Y: Sorc
    Verbose("Time mapping for HNM composition...\n");
    _PMatch TimeMatch;
    RCall(_PMatch, Ctor)(& TimeMatch);
    RCall(_PMatch, AddPair)(& TimeMatch, 0, 0);
    RCall(_PMatch, AddPair)(& TimeMatch, SorcDB -> VOT, SorcDB -> VOT);
    
    Segmentation Orig, Nseg;
    Orig.P1 = (Real)(SorcDB -> InvarLeft  - SorcDB -> VOT) / SampleRate;
    Orig.P2 = (Real)(SorcDB -> InvarRight - SorcDB -> VOT) / SampleRate;
    Orig.P3 = (Real)(SorcSize - SorcDB -> VOT) / SampleRate;
    
    Nseg = Resegment(Orig, (Real)Dest -> Size / SampleRate,
        Para -> FlagPara.DeltaSeg1, Para -> FlagPara.DeltaSeg2);
    if(Nseg.P1 == Nseg.P2)
    {
        RCall(_PMatch, AddPair)(& TimeMatch,
            SorcDB -> VOT + SampleRate * 0.9 * Nseg.P1,
            SorcDB -> InvarLeft);
        RCall(_PMatch, AddPair)(& TimeMatch,
            SorcDB -> VOT + SampleRate * 1.1 * Nseg.P1,
            SorcDB -> InvarRight);
    }else
    {
        RCall(_PMatch, AddPair)(& TimeMatch,
            SorcDB -> VOT + Nseg.P1 * SampleRate,
            SorcDB -> InvarLeft);
        RCall(_PMatch, AddPair)(& TimeMatch,
            SorcDB -> VOT + Nseg.P2 * SampleRate,
            SorcDB -> InvarRight);
    }
    
    RCall(_PMatch, AddPair)(& TimeMatch, Dest -> Size, SorcSize);
    RCall(_PMatch, Print)(& TimeMatch);
    
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
    RCall(_HNMContour, Dtor)(& TempCont);
    
    //Smoothen
    #define DecayRate 0.8
    #define DecayLen  5
    Verbose("Smoothening transition...\n");
    int CenterPos = Nseg.P1 * SampleRate + SorcDB -> VOT;
    int CenterIndex = CDSP2_List_Int_IndexAfter(
                        & VowSynth.PulseList, CenterPos);
    int LDecay = CenterIndex - DecayLen > 0 ? CenterIndex - 5 : 1;
    int HDecay = CenterIndex + DecayLen < VowSynth.PulseList.Frames_Index
               ? CenterIndex + DecayLen : VowSynth.PulseList.Frames_Index - 1;
    for(i = LDecay; i < HDecay; i ++)
    {
        RCall(_HNMFrame, From)(& TempHNM, & VowSynth.HNMList.Frames[i]);
        InterpHNMFrame(& VowSynth.HNMList.Frames[i    ],
            & TempHNM, & VowSynth.HNMList.Frames[i - 1], DecayRate);
    }
    
    RCall(_HNMFrame, Dtor)(& TempHNM);
    
    //HNM synthesis
    Verbose("HNM synthesis...\n");
    Verbose("%d HNM frames.\n", VowSynth.PulseList.Frames_Index + 1);
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
    Verbose("Concatenating at %d...\n", ConcatPos);
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
    RCall(_PMatch, Dtor)(& F0List);
    RDelete(& DyWin, & ConWave, & VowWave, & NozWave,
        & VowList, & PhseList, & VowPulse, & VowMatch, & TimeMatch,
        & VowSynth);
    
    return 1;
}

