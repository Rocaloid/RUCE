#include "Synth.h"
#include "Verbose.h"

static int LoadHNMFrame(List_HNMFrame* Dest, List_DataFrame* DestPhse,
    RUCE_DB_Entry* Sorc, int WinSize)
{
    Verbose(4, "(function entrance)\n");
    int FrameNum = Sorc -> FrameList_Index + 1;
    
    return 1;
}

static int MatchUnitToNote(PMatch* Dest, RUCE_DB_Entry* SorcDB,
    RUCE_Session* Session, int NoteIndex)
{
    Verbose(4, "(function entrance)\n");
    return 1;
}

static void LoadPulseTrain(List_Int* Dest, RUCE_DB_Entry* Sorc)
{
    Verbose(4, "(function entrance)\n");
    Dest -> Frames_Index = Sorc -> FrameList_Index;
    for(int i = 0; i <= Sorc -> FrameList_Index; i ++)
        Dest -> Frames[i] = Sorc -> FrameList[i].Position;
}

//Returns the alignment in index.
//  1: Success
//  0: Cannot load HNMFrame
// -1: Cannot match phoneme durations
int RUCE_VoicedToHNMContour(List_HNMContour* Dest, List_DataFrame* DestPhse,
    RUCE_DB_Entry* SorcDB, CSVP_PitchModel* SorcPM, RUCE_Session* Session,
    int NoteIndex)
{
    RUCE_SessionConfig* Config = Session -> Config;
    RUCE_Note* Note = & Session -> NoteList[NoteIndex];
    double NoteTime = Session -> TimeList[NoteIndex];
    int SampleRate = Session -> SampleRate;
    
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
        
              0 ST0    STV                 STD
        Src   |--|------|-------------------|
            0  DT0     DTV                   DTD
        Dst |---|-------|---------------------|
        
        ST0/DT0: First HNM frame
        STV/DTV: VOT/Syllable Alignment
        STD/DTD: End/Duration
    */
    #define Sample2Sec(x) ((double)(x) / SampleRate)
    #define Sec2Sample(x) (SampleRate * (x))
    int SorcHead = SorcDB -> FrameList[0].Position;
    int HopSize  = SorcDB -> HopSize;
    double STV = SorcDB -> VOT;
    double DTV = Note -> CParamSet.DurConsonant <= 0 ?
        SorcDB -> VOT : Note -> CParamSet.DurConsonant;
    
    PMatch TimeMatch, RevTimeMatch;
    RCall(PMatch, Ctor)(& TimeMatch);
    RCall(PMatch, Ctor)(& RevTimeMatch);
    if(MatchUnitToNote(& TimeMatch, SorcDB, Session, NoteIndex) < 1)
    {
        Verbose(1, "[Error] Cannot match phoneme durations.\n");
        RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
            & SorcTrainMatch);
        return -1;
    }
    //RCall(PMatch, InvertTo)(& TimeMatch, & RevTimeMatch);
    
    double ST0 = Sample2Sec(SorcHead);
    double DT0 = RCall(PMatch, Query)(& RevTimeMatch, ST0).Y;
    double STD = Sample2Sec(TopOf(SorcDB -> FrameList).Position);
    double DTD = Note -> Duration + DT0;
    Verbose(5, "ST0=%f, STV=%f, STD=%f, DT0=%f, DTV=%f, DTD=%f\n",
        ST0, STV, STD, DT0, DTV, DTD);
    
    #define MapToSource(n) \
        Sec2Sample(RCall(PMatch, Query)(& TimeMatch, Sample2Sec(n) + DT0).Y)
    #define MapToGlobal(n) \
        (Sample2Sec(n) + DT0 - DTV + NoteTime)
    
    /*
        Step3: Transform
    */
    int Position = 0;
    int LocalDuration = SampleRate * DTD;
    while(Position < LocalDuration)
    {
        int SourcePosition = MapToSource(Position);
        Transition Trans   = RCall(PMatch, Query)(& SorcTrainMatch,
            SourcePosition);
        Verbose(5, "%d %d %f\n", Position, SourcePosition, Trans.Y);
        Position += HopSize;
    }
    
    /*
        Step4: Post-transform
    */
    
    RDelete(& SorcHNM, & SorcPhase, & TimeMatch, & RevTimeMatch,
        & SorcTrainMatch);
    return 1;
}

