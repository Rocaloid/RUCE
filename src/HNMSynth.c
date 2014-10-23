#include "Synth.h"
#include "Verbose.h"

typedef struct
{
    double T0, TV, T1, T2, TD;
} Segmentation;

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

/*
                     (SP1) (SP2)
          0 ST0    STV  ST1     ST2    STD
    Src   |--|------|----|-------|------|
                     (DP1)  (DP2)
        0  DT0     DTV   DT1     DT2     DTD
    Dst |---|-------|-----|-------|-------|
    
    ST0/DT0: First HNM frame
    ST1/DT1: Initial of voiced part
    ST2/DT2: Final of voiced part
    STV/DTV: VOT/Syllable alignment
    STD/DTD: End/Duration
*/
#define Sample2Sec(x) ((double)(x) / SampleRate)
#define Sec2Sample(x) (SampleRate * (x))
static Segmentation MatchUnitToNote(PMatch* Dest, RUCE_DB_Entry* SorcDB,
    RUCE_Session* Session, int NoteIndex)
{
    Verbose(4, "(function entrance)\n");
    Segmentation Ret;
    int SampleRate = Session -> SampleRate;
    int SorcHead = SorcDB -> FrameList[0].Position;
    RUCE_Note* Note = & Session -> NoteList[NoteIndex];
    
    double ST0 = Sample2Sec(SorcHead);
    double STV = SorcDB -> VOT;
    double DTV = Note -> CParamSet.DurConsonant <= 0 ? SorcDB -> VOT :
        Note -> CParamSet.DurConsonant;
    double STD = Sample2Sec(TopOf(SorcDB -> FrameList).Position);
    double DTD = Note -> Duration + Note -> CParamSet.DeltaDuration + DTV;
    
    //ST0 -> DT0, STV -> DTV, STD -> DTD
    RCall(PMatch, AddPair)(Dest, 0, 0);
    RCall(PMatch, AddPair)(Dest, DTV, STV);
    RCall(PMatch, AddPair)(Dest, DTD, STD);
    
    //ST1 -> DT1, ST2 -> DT2
    double SP1 = SorcDB -> InvarLeft - STV;
    double SP2 = SorcDB -> InvarRight - STV;
    double DP1, DP2;
    
    if(Note -> CParamSet.DurInitial > 0)
        DP1 = Note -> CParamSet.DurInitial;
    if(Note -> CParamSet.DurFinal > 0)
        DP2 = STD - Note -> CParamSet.DurFinal - STV;
    
    //Final not determined yet.
    if(Note -> CParamSet.DurInitial > 0 && Note -> CParamSet.DurFinal <= 0)
    {
        DP2 = DTD - (STD - SP2);
        //Prevent over-long finals.
        if(DP2 - DP1 < DTD - DP2)
            DP2 = DP1 + (DTD - DP1) / 2.0;
    }
    
    //Initial not determined yet.
    if(Note -> CParamSet.DurFinal > 0 && Note -> CParamSet.DurInitial <= 0)
    {
        DP1 = SP1;
        //Prevent over-long initials.
        if(DP1 > DP2 - DP1)
            DP1 = DP2 / 2;
    }
    
    //Both final & initial not determined yet.
    if(Note -> CParamSet.DurFinal <= 0 && Note -> CParamSet.DurInitial <= 0)
    {
        DP1 = SP1;
        DP2 = DTD - (STD - SP2);
        
        if(DP2 < DP1)
        {
            DP1 = DP2 = SP1 / (SP1 + SP2) * DTD;
            DP1 *= 0.7;
            DP2 = DTD - (DTD - DP2) * 0.7;
        }
    }
    
    RCall(PMatch, AddPair)(Dest, DTV + DP1, STV + SP1);
    RCall(PMatch, AddPair)(Dest, DTV + DP2, STV + SP2);
    
    PMatch RevTimeMatch;
    RCall(PMatch, Ctor)(& RevTimeMatch);
    RCall(PMatch, InvertTo)(Dest, & RevTimeMatch);
    Ret.T0 = RCall(PMatch, Query)(& RevTimeMatch, ST0).Y;
    Ret.TV = DTV;
    Ret.T1 = DTV + DP1;
    Ret.T2 = DTV + DP2;
    Ret.TD = DTD;
    
    RCall(PMatch, Dtor)(& RevTimeMatch);
    
    return Ret;
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
// >0: Success
//  0: Cannot load HNMFrame
// -1: Cannot match phoneme durations
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
        return 0;
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
            return -1;
        }
        if(ParamAdjustPhase(& TempPhase, SorcPM, F0, Amp, Bre, Gen) < 1)
        {
            Verbose(1, "[Error] Phase adjustment failed.\n");
            RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
                & SorcTrainMatch, & TempFrame, & TempContour, & TempPhase);
            return -2;
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
    SmoothenContourList(Dest, D.T1 - D.T0, HopSize, SampleRate, Note);
    
    RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
        & SorcTrainMatch);
    int Ret = (D.TV - D.T0) * SampleRate / HopSize;
    Verbose(3, "Ret=%d\n", Ret);
    return Ret;
}

//Returns the alignment in samples
// >0: Success
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
    RCall(Wave, Resize)(DestHmnc, N * HopSize + SorcConfig -> WinSize);
    RCall(Wave, Resize)(DestNoiz, N * HopSize + SorcConfig -> WinSize);
    
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
            8000);
        RCall(HNMItersizer, Add)(& Synth, & TempHNM, i * HopSize);
        if(i % 10 == 0)
            RCall(HNMItersizer, AddPhase)(& Synth, & SorcPhse -> Frames[i],
                i * HopSize);
    }
    
    RCall(HNMItersizer, SetPosition)(& Synth, 500);
    RCall(HNMItersizer, SetInitPhase)(& Synth, & SorcPhse -> Frames[2]);
    RCall(HNMItersizer, IterNextTo)(& Synth, (N - 1) * HopSize);
    
    RDelete(& TempHNM, & Synth);
    
    return 1;
}

