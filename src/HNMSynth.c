#include "Synth.h"
#include "Timing.h"
#include "Verbose.h"

static int LoadHNMFrame(List_HNMFrame* Dest, List_DataFrame* DestPhse,
    RUCE_DB_Entry* Sorc, int WinSize)
{
    Verbose(4, "(function entrance)\n");
    int FrameNum = Sorc -> FrameList_Index + 1;
    RCall(List_HNMFrame, Resize)(Dest, FrameNum);
    RCall(List_DataFrame, Resize)(DestPhse, FrameNum);
    Verbose(4, "%d HNMFrames were found.\n", FrameNum);
    
    for(int i = 0; i < FrameNum; i ++)
    {
        int HNum;
        
        RUCE_DB_Frame* SorcFrame = & Sorc -> FrameList[i];
        HNMFrame* DestFrame = & Dest -> Frames[i];
        HNum = SorcFrame -> Freq_Index + 1;
        
        RCall(HNMFrame, Resize)(DestFrame, WinSize, HNum);
        RCall(DataFrame, Resize)(& DestPhse -> Frames[i], HNum);
        
        for(int j = 0; j < HNum; j ++)
        {
            DestFrame -> Hmnc.Freq[j] = SorcFrame -> Freq[j];
            DestFrame -> Hmnc.Ampl[j] = SorcFrame -> Ampl[j];
            DestPhse -> Frames[i].Data[j] = SorcFrame -> Phse[j];
        }
        
        RCall(CDSP2_VSet, Real)(DestFrame -> Noiz, -999, WinSize / 2 + 1);
        RCall(CDSP2_Resample_Linear, Real)(DestFrame -> Noiz, SorcFrame -> Noiz,
            WinSize / 2 + 1, Sorc -> NoizSize);
    }
    
    return 1;
}

static void InterpFetchHNMFrame(HNMFrame* Dest, List_HNMFrame* Sorc,
    Transition* Trans)
{
    int LIndex, HIndex;
    LIndex = Trans -> LowerIndex;
    HIndex = Trans -> LowerIndex == Sorc -> Frames_Index ? LIndex : LIndex + 1;
    HNMFrame* LHNM = & Sorc -> Frames[LIndex];
    HNMFrame* HHNM = & Sorc -> Frames[HIndex];
    
    if(Trans -> X < 0.0) Trans -> X = 0;
    if(Trans -> X > 1.0) Trans -> X = 1.0;
    RCall(HNMFrame, InterpFrom)(Dest, LHNM, HHNM, Trans -> X);
}

static void InterpFetchPhase(DataFrame* Dest, List_DataFrame* Sorc,
    Transition* Trans)
{
    int LIndex, HIndex;
    LIndex = Trans -> LowerIndex;
    HIndex = Trans -> LowerIndex == Sorc -> Frames_Index ? LIndex : LIndex + 1;
    DataFrame* LPhase = & Sorc -> Frames[LIndex];
    DataFrame* HPhase = & Sorc -> Frames[HIndex];
    
    DataFrame SLPhase, SHPhase;
    RCall(DataFrame, Ctor)(& SLPhase);
    RCall(DataFrame, Ctor)(& SHPhase);
    
    RCall(DataFrame, From)(& SLPhase, LPhase);
    RCall(DataFrame, From)(& SHPhase, HPhase);
    RCall(CSVP_PhaseSyncH, Real)(& SLPhase, 0);
    RCall(CSVP_PhaseSyncH, Real)(& SHPhase, 0);
    RCall(CSVP_PhaseInterp, Real)(Dest, & SLPhase, & SHPhase, Trans -> X);
    
    RCall(DataFrame, Dtor)(& SLPhase);
    RCall(DataFrame, Dtor)(& SHPhase);
}

static void LoadPulseTrain(List_Int* Dest, RUCE_DB_Entry* Sorc)
{
    Verbose(4, "(function entrance)\n");
    Dest -> Frames_Index = Sorc -> FrameList_Index;
    for(int i = 0; i <= Sorc -> FrameList_Index; i ++)
        Dest -> Frames[i] = Sorc -> FrameList[i].Position;
}

static int ParamConvertHNM(HNMContour* Dest, HNMFrame* Sorc,
    CSVP_PitchModel* PM, int SampleRate,
    double F0, double Amp, double Bre, double Gen)
{
    Verbose(6, "(function entrance)\n");
    HNMFrame Shifted;
    RCall(HNMFrame, Ctor)(& Shifted);
    RCall(HNMFrame, From)(& Shifted, Sorc);
    
    for(int i = 0; i <= Shifted.Hmnc.Freq_Index; i ++)
        Shifted.Hmnc.Freq[i] *= Gen + 0.5;
    
    CSVP_PitchConvertHNMFrame_Float(Dest, & Shifted, PM, F0, 10000, SampleRate);
    
    float NoizAdj = log(Amp);
    float HmncAdj = log(Amp);
    
    if(fabs(Bre - 0.5) > 0.001)
        NoizAdj += Bre < 0.001 ? -999.0 : log(Bre * 2.0);
    if(Bre > 0.501)
        HmncAdj += Bre > 0.999 ? -999.0 : log(2.0 - Bre * 2.0);
    
    Verbose(6, "NoizAdj=%f, HmncAdj=%f\n", NoizAdj, HmncAdj);
    RCall(CDSP2_VCAdd, Real)(Dest -> Noiz, Dest -> Noiz, NoizAdj,
        Dest -> Size / 2 + 1);
    RCall(CDSP2_VCAdd, Real)(Dest -> Hmnc, Dest -> Hmnc, HmncAdj,
        Dest -> Size / 2 + 1);
        
    RCall(HNMFrame, Dtor)(& Shifted);
    return 1;
}

static int ParamAdjustPhase(DataFrame* Dest, CSVP_PitchModel* PM,
    double F0, double Amp, double Bre, double Gen)
{
    RCall(CSVP_PhaseContract, Real)(Dest, CSVP_PitchModel_GetPhseCoh(PM, F0));
    return 1;
}

static void SmoothenContourList(List_HNMContour* Dest, double Center,
    int HopSize, int SampleRate, RUCE_Note* Note)
{
    Verbose(4, "(function entrance)\n");
    
    const double DecayTime = 0.06;
    const double DecayRate = 0.6;
    
    int LDecayIndex = (Center - DecayTime) * SampleRate / HopSize;
    int HDecayIndex = (Center + DecayTime) * SampleRate / HopSize;
    LDecayIndex = LDecayIndex < 1 ? 1 : LDecayIndex;
    HDecayIndex = HDecayIndex > Dest -> Frames_Index ? Dest -> Frames_Index :
        HDecayIndex;
    
    Verbose(4, "Center=%f, LDecayIndex=%d, HDecayIndex=%d\n", Center,
        LDecayIndex, HDecayIndex);
    
    HNMContour TempCont;
    RCall(HNMContour, Ctor)(& TempCont);
    for(int i = LDecayIndex; i < HDecayIndex; i ++)
    {
        RCall(HNMContour, From)(& TempCont, & Dest -> Frames[i]);
        RCall(HNMContour, InterpFrom)(& Dest -> Frames[i], & TempCont,
            & Dest -> Frames[i - 1], DecayRate);
    }
    
    RCall(HNMContour, Dtor)(& TempCont);
}

//Returns the alignment in index.
// >=0: Success
//  -1: Cannot load HNMFrame
//  -2: Cannot match phoneme durations
int RUCE_VoicedToHNMContour(List_HNMContour* Dest, List_DataFrame* DestPhse,
    Real* DestF0, RUCE_DB_Entry* SorcDB, CSVP_PitchModel* SorcPM,
    RUCE_Session* Session, int NoteIndex)
{
    Verbose(3, "(function entrance)\n");
    RUCE_SessionConfig* Config = Session -> Config;
    RUCE_Note* Note = & Session -> NoteList[NoteIndex];
    double NoteTime = Session -> TimeList[NoteIndex];
    int SampleRate = Session -> SampleRate;
    int HopSize  = SorcDB -> HopSize;
    
    List_HNMFrame  SorcHNM;
    List_DataFrame SorcPhase;
    List_Int       SorcTrain;
    PMatch         SorcTrainMatch;
    RCall(List_HNMFrame, Ctor)(& SorcHNM);
    RCall(List_DataFrame, Ctor)(& SorcPhase);
    
    /*
        Step1: Load
    */
    if(LoadHNMFrame(& SorcHNM, & SorcPhase, SorcDB, Config -> WinSize) < 1)
    {
        Verbose(1, "[Error] Cannot load HNMFrame.\n");
        RDelete(& SorcHNM, & SorcPhase);
        return -1;
    }
    RCall(List_Int, CtorSize)(& SorcTrain, SorcDB -> FrameList_Index + 1);
    LoadPulseTrain(& SorcTrain, SorcDB);
    RCall(PMatch, Ctor)(& SorcTrainMatch);
    RCall(CDSP2_List_Int_ToPMatch, Real)(& SorcTrain, & SorcTrainMatch);
    RCall(List_Int, Dtor)(& SorcTrain);
    
    /*
        Step2: Match
    */
    PMatch TimeMatch, RevTimeMatch;
    RCall(PMatch, Ctor)(& TimeMatch);
    RCall(PMatch, Ctor)(& RevTimeMatch);
    Segmentation D = MatchUnitToNote(& TimeMatch, SorcDB, Session, NoteIndex);
    
    #define MapToSource(n) \
        Sec2Sample(RCall(PMatch, Query)(& TimeMatch, Sample2Sec(n) + D.T0).Y)
    #define MapToGlobal(n) \
        (Sample2Sec(n) + D.T0 - D.TV + NoteTime)
    
    /*
        Step3: Transform
    */
    HNMFrame TempFrame;
    HNMContour TempContour;
    DataFrame TempPhase;
    RCall(HNMFrame, Ctor)(& TempFrame);
    RCall(HNMContour, Ctor)(& TempContour);
    RCall(DataFrame, Ctor)(& TempPhase);
    int Position = 0;
    int Count = 0;
    int LocalDuration = SampleRate * (D.TD - D.T0);
    while(Position < LocalDuration)
    {
        int SourcePosition = MapToSource(Position);
        Transition Trans   = RCall(PMatch, Query)(& SorcTrainMatch,
            SourcePosition);
        double GlobalTime  = MapToGlobal(Position);
        Verbose(6, "%d %d %f %s\n", Position, Trans.LowerIndex, GlobalTime,
            Sample2Sec(Position) > D.TV ? "V" : "U");
        
        double F0  = RCall(PMatch, Query)(Session -> FreqMatch  , GlobalTime).Y;
        double Amp = RCall(PMatch, Query)(Session -> AmplMatch  , GlobalTime).Y;
        double Bre = RCall(PMatch, Query)(Session -> BreMatch   , GlobalTime).Y;
        double Gen = RCall(PMatch, Query)(Session -> GenderMatch, GlobalTime).Y;
        Verbose(6, "F0=%f, Amp=%f, Bre=%f, Gen=%f\n", F0, Amp, Bre, Gen);
        
        InterpFetchHNMFrame(& TempFrame, & SorcHNM, & Trans);
        InterpFetchPhase(& TempPhase, & SorcPhase, & Trans);
        
        if(ParamConvertHNM(& TempContour, & TempFrame, SorcPM, SampleRate,
            F0, Amp, Bre, Gen) < 1)
        {
            Verbose(1, "[Error] HNM contour conversion failed.\n");
            RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
                & SorcTrainMatch, & TempFrame, & TempContour, & TempPhase);
            return -2;
        }
        if(ParamAdjustPhase(& TempPhase, SorcPM, F0, Amp, Bre, Gen) < 1)
        {
            Verbose(1, "[Error] Phase adjustment failed.\n");
            RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
                & SorcTrainMatch, & TempFrame, & TempContour, & TempPhase);
            return -3;
        }
        
        RCall(List_HNMContour, Add)(Dest, & TempContour, Count);
        RCall(List_DataFrame, Add)(DestPhse, & TempPhase, Count);
        DestF0[Count] = F0;
        Count ++;
        Position += HopSize;
    }
    RDelete(& TempFrame, & TempContour, & TempPhase);
    
    /*
        Step4: Post-conversion transforms
    */
    //SmoothenContourList(Dest, D.T1 - D.T0, HopSize, SampleRate, Note);
    
    RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
        & SorcTrainMatch);
    int Ret = (D.TV - D.T0) * SampleRate / HopSize;
    Verbose(3, "Ret=%d\n", Ret);
    return Ret;
}

//  1: Success
//  0: Failed, sizes of source lists mismatch.
// -1: Failed, undocumented error.
int RUCE_SynthHNMContour(Wave* DestHmnc, Wave* DestNoiz, List_HNMContour* Sorc,
    List_DataFrame* SorcPhse, Real* SorcF0, int HopSize,
    RUCE_SessionConfig* SorcConfig)
{
    Verbose(3, "(function entrance)\n");
    if(SorcPhse -> Frames_Index != Sorc -> Frames_Index)
    {
        Verbose(1, "[Error] Sizes of source lists mismatch.\n");
        return 0;
    }
    
    int N = Sorc -> Frames_Index + 1;
    int DestSize = N * HopSize + SorcConfig -> WinSize;
    Verbose(3, "N=%d, DestSize=%d\n", N, DestSize);
    RCall(Wave, Resize)(DestHmnc, DestSize);
    RCall(Wave, Resize)(DestNoiz, DestSize);
    RCall(CDSP2_VSet, Real)(DestHmnc -> Data, 0, DestSize);
    RCall(CDSP2_VSet, Real)(DestNoiz -> Data, 0, DestSize);
    
    HNMItersizer Synth;
    RCall(HNMItersizer, CtorSize)(& Synth, SorcConfig -> WinSize);
    RCall(HNMItersizer, SetHopSize)(& Synth, HopSize);
    RCall(HNMItersizer, SetWave)(& Synth, DestHmnc);
    RCall(HNMItersizer, SetNoizWave)(& Synth, DestNoiz);
    
    HNMFrame TempHNM;
    RCall(HNMFrame, Ctor)(& TempHNM);
    for(int i = 0; i < N; i ++)
    {
        RCall(HNMFrame, FromContour)(& TempHNM, & Sorc -> Frames[i], SorcF0[i],
            SorcConfig -> HmncUpperBound);
        RCall(HNMItersizer, Add)(& Synth, & TempHNM, i * HopSize);
        if(i % 10 == 0)
            RCall(HNMItersizer, AddPhase)(& Synth, & SorcPhse -> Frames[i],
                i * HopSize);
    }
    
    int SynthDest = (N - 1) * HopSize;
    RCall(HNMItersizer, SetPosition)(& Synth, 0);
    RCall(HNMItersizer, SetInitPhase)(& Synth, & SorcPhse -> Frames[0]);
    RCall(HNMItersizer, IterNextTo)(& Synth, SynthDest);
    
    //Fade-in & Fade-out
    for(int i = 0; i < 100; i ++)
    {
        DestHmnc -> Data[i] *= (Real)i / 100;
        DestNoiz -> Data[i] *= (Real)i / 100;
    }
    for(int i = 0; i < 100; i ++)
    {
        DestHmnc -> Data[i + SynthDest - 100] *= 1.0 - (Real)i / 100;
        DestNoiz -> Data[i + SynthDest - 100] *= 1.0 - (Real)i / 100;
    }
    for(int i = (N - 1) * HopSize; i < DestSize; i ++)
    {
        DestHmnc -> Data[i] = 0;
        DestNoiz -> Data[i] = 0;
    }
    
    RDelete(& TempHNM, & Synth);
    
    return 1;
}

