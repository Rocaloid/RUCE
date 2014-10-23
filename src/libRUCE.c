#include "libRUCE.h"
#include "Synth.h"
#include "SessionConfig.h"
#include <RUtil2.h>
#include "Config.h"
#include "Verbose.h"
#include "Common.h"

#define Modify(Type, Name) (*((Type*)(& (Name))))
int _RUCE_VerboseLevel = 2;

RCtor(RUCE_SessionConfig)
{
    This -> Window = RCall(RAlloc, Real)(2048);
    This -> WinSize = 2048;
    RCall(CDSP2_GenHanning, Real)(This -> Window, 2048);
    RInit(RUCE_SessionConfig);
}

RDtor(RUCE_SessionConfig)
{
    RFree(This -> Window);
}

const RUCE_Version Version = {
    RUCE_VERSION_MAJOR, RUCE_VERSION_MINOR, RUCE_VERSION_REVISION,
    RUCE_VERSION_PATCH, RUCE_VERSION_TAG,
    RUCE_APIMAJOR, RUCE_APIMINOR, RUCE_APIREVISION};

const RUCE_Version* RUCE_GetVersion()
{
    return & Version;
}

void RUCE_SetVerboseLevel(int Level)
{
    _RUCE_VerboseLevel = Level;
}

RUCE_Note* RUCE_CreateNote()
{
    RUCE_Note* Ret = (RUCE_Note*)malloc(sizeof(RUCE_Note));
    
    Ret -> Duration = 0;
    Ret -> Lyric = (char*)malloc(RUCE_LYRICSIZE);
    memset(Ret -> Lyric, 0, RUCE_LYRICSIZE);
    Ret -> CParamSet.DeltaOnset = 0;
    Ret -> CParamSet.DeltaDuration = 0;
    Ret -> CParamSet.DurFinal = 0;
    Ret -> CParamSet.DurInitial = 0;
    Ret -> CParamSet.DurConsonant = 0;
    Ret -> CParamSet.OffsetConsonant = 0;
    Ret -> CParamSet.AmplConsonant = 0;
    Ret -> PhaseSync = 1.0;
    
    return Ret;
}

void RUCE_Note_Dtor(RUCE_Note* This)
{
    free(This -> Lyric);
}

void RUCE_Note_CtorFrom(RUCE_Note* This, RUCE_Note* Sorc)
{
    int Size = strlen(Sorc -> Lyric);
    *This = *Sorc;
    This -> Lyric = (char*)malloc(Size + 1);
    strcpy(This -> Lyric, Sorc -> Lyric);
}

int RUCE_DestroyNote(RUCE_Note* Note)
{
    RUCE_Note_Dtor(Note);
    free(Note);
    return 1;
}

RUCE_Session* RUCE_CreateSynthSession(int SampleRate, int BufferSize)
{
    RUCE_Session* Ret = (RUCE_Session*)malloc(sizeof(RUCE_Session));
    Ret -> Config = (RUCE_SessionConfig*)malloc(sizeof(RUCE_SessionConfig));
    RUCE_SessionConfig_Ctor(Ret -> Config);
    
    Ret -> Buffer = RAlloc_Class(InfWave, 1);
    Ret -> FreqMatch = RAlloc_Class(PMatch, 1);
    Ret -> AmplMatch = RAlloc_Class(PMatch, 1);
    Ret -> BreMatch  = RAlloc_Class(PMatch, 1);
    Ret -> GenderMatch = RAlloc_Class(PMatch, 1);
    
    RCall(PMatch, Ctor)(Ret -> FreqMatch);
    RCall(PMatch, Ctor)(Ret -> AmplMatch);
    RCall(PMatch, Ctor)(Ret -> BreMatch);
    RCall(PMatch, Ctor)(Ret -> GenderMatch);
    
    Modify(int, Ret -> SampleRate) = SampleRate;
    Modify(int, Ret -> SynthHead)  = 0;
    
    RCall(InfWave, CtorSize)(Ret -> Buffer, BufferSize);
    Array_Ctor(RUCE_Note, Ret -> NoteList);
    Array_Ctor(double, Ret -> TimeList);
    
    return Ret;
}

int RUCE_DestroySynthSession(RUCE_Session* Session)
{
    RCall(InfWave, Dtor)(Session -> Buffer);
    RUCE_SessionConfig_Dtor(Session -> Config);
    free(Session -> Config);
    
    Array_ObjDtor(RUCE_Note, Session -> NoteList);
    Array_Dtor(RUCE_Note, Session -> NoteList);
    Array_Dtor(double, Session -> TimeList);
    RCall(PMatch, Dtor)(Session -> FreqMatch);
    RCall(PMatch, Dtor)(Session -> AmplMatch);
    RCall(PMatch, Dtor)(Session -> BreMatch);
    RCall(PMatch, Dtor)(Session -> GenderMatch);
    RFree(Session -> Buffer);
    RFree(Session -> FreqMatch);
    RFree(Session -> AmplMatch);
    RFree(Session -> BreMatch);
    RFree(Session -> GenderMatch);
    free(Session);
    return 1;
}

void RUCE_SessionSetSoundbank(RUCE_Session* Session, RUCE_Soundbank* Sorc)
{
    Session -> Soundbank = Sorc;
}

int RUCE_SessionAppendNote(RUCE_Session* Session, RUCE_Note* SorcNote,
    double Time)
{
    if((double)Session -> SynthHead / Session -> SampleRate >= Time)
        return -2;
    if(Session -> TimeList_Index >= 0 && TopOf(Session -> TimeList) >= Time)
        return -2;
    if(SorcNote -> Duration > (double)((Wave*)Session -> Buffer) -> Size / 2.0)
        return -1;
    Array_Push(double, Session -> TimeList, Time);
    Array_PushNull(RUCE_Note, Session -> NoteList);
    RUCE_Note_CtorFrom(& TopOf(Session -> NoteList), SorcNote);
    return 1;
}

int RUCE_SessionAppendEvent(RUCE_Session* Session, int Type, double Time,
    double Value)
{
    if((double)Session -> SynthHead / Session -> SampleRate >= Time)
        return -2;
    switch(Type)
    {
    case RUCE_EFREQ:
        if(Value <= 0 || Value > 2000.0) return -3;
        RCall(PMatch, AddPair)(Session -> FreqMatch, Time, Value);
    break;
    case RUCE_EAMPL:
        if(Value < 0) return -3;
        RCall(PMatch, AddPair)(Session -> AmplMatch, Time, Value);
    break;
    case RUCE_EBRE:
        if(Value < 0 || Value > 1) return -3;
        RCall(PMatch, AddPair)(Session -> BreMatch, Time, Value);
    break;
    case RUCE_EGEN:
        if(Value < -1 || Value > 1) return -3;
        RCall(PMatch, AddPair)(Session -> GenderMatch, Time, Value);
    break;
    default:
        return -1;
    }
    
    return 1;
}

int RUCE_SessionSynthInit(RUCE_Session* Session, double TimeStart)
{
    //Remove notes end before TimeStart.
    int RmIndex, i;
    RmIndex = -1;
    for(i = 0; i <= Session -> TimeList_Index; i ++)
        if(Session -> TimeList[i] + Session -> NoteList[i].Duration > TimeStart)
        {
            RmIndex = i - 1;
            break;
        }
    if(RmIndex >= 0)
    {
        for(i = 0; i <= RmIndex; i ++)
            RUCE_Note_Dtor(& Session -> NoteList[i]);
        Array_RemoveRange(double, Session -> TimeList, 0, RmIndex);
        Array_RemoveRange(RUCE_Note, Session -> NoteList, 0, RmIndex);
    }
    for(i = 0; i <= Session -> TimeList_Index; i ++)
        Verbose(4, "%f -> %f\n",
            Session -> TimeList[i],
            Session -> TimeList[i] + Session -> NoteList[i].Duration);
    
    //Check validity of TimeStart.
    if(TopOf(Session -> TimeList) + TopOf(Session -> NoteList).Duration
     < TimeStart)
        return -1;
    
    Modify(int, Session -> SynthHead) = TimeStart * Session -> SampleRate;
    RCall(InfWave, Relocate)(Session -> Buffer, Session -> SynthHead);
    
    return 1;
}

#define Sample2Sec(x) ((double)(x) / Session -> SampleRate)
#define Sec2Sample(x) (Session -> SampleRate * (x))
#define T(n) (Session -> TimeList[n])
#define D(n) (Session -> NoteList[n].Duration)
#define N(n) (Session -> NoteList[n])

// Synthesize to a certain time point(Time) and dump audio into DestBuffer.
// Returns number of samples dumped into DestBuffer for success, 0 or negative
//   integer for failure.
//   0: Undocumented error.
//  -1: Invalid Time.
//  -2: Synthesis succeeded, but one or more notes are not pronounced.
int RUCE_SessionSynthStep(RUCE_Session* Session, Real* DestBuffer,
    double Time)
{
    Verbose(3, "SynthHead before this step: %d.\n", Session -> SynthHead);
    Wave UnvoicedWave, VoicedWave, NoiseWave;
    RUCE_DB_Entry DBEntry;
    CSVP_PitchModel PMEntry;
    String UnitName;
    RNew(Wave, & UnvoicedWave, & VoicedWave, & NoiseWave);
    String_Ctor(& UnitName);
    
    Real* F0List = RCall(RAlloc, Real)(16384);
    RUCE_SessionConfig* Config = (RUCE_SessionConfig*)Session -> Config;
    UnvoicedWave.SampleRate = Session -> SampleRate;
    VoicedWave.SampleRate   = Session -> SampleRate;
    NoiseWave.SampleRate    = Session -> SampleRate;
    RCall(Wave, SetWindow)(& UnvoicedWave, Config -> Window, Config -> WinSize);
    RCall(Wave, SetWindow)(& VoicedWave, Config -> Window, Config -> WinSize);
    RCall(Wave, SetWindow)(& NoiseWave, Config -> Window, Config -> WinSize);
    
    int i = 0, Ret = 1;
    int AlignHead = Session -> SynthHead;
    if(Session -> TimeList_Index >= 0 && AlignHead > Sec2Sample(T(0)))
        AlignHead = Sec2Sample(T(0));
    while(Sample2Sec(AlignHead) < Time + 0.5)
    {
        if(i > Session -> NoteList_Index)
            break;
        
        //Initiate untouched area with zeros.
        int DestHead = Sec2Sample(T(i) + D(i));
        int DestLen  = DestHead - Session -> SynthHead;
        RCall(CDSP2_VSet, Real)(
            RCall(InfWave, GetUnsafePtr)(Session -> Buffer) + Session ->
            SynthHead, 0, DestLen);
        
        //Load PitchModel & DBEntry
        CSVP_PitchModel_Ctor(& PMEntry);
        RUCE_DB_Entry_Ctor(& DBEntry);
        String_SetChars(& UnitName, N(i).Lyric);
        if(RUCE_SoundbankLoadEntry(& DBEntry, Session -> Soundbank, & UnitName)
            != 1)
        {
            Ret = -2;
            Verbose(2, "[Warning] Cannot load '%s'. Skipped.\n",
                String_GetChars(& UnitName));
            goto SkipSynth;
        }
        if(DBEntry.Samprate != Session -> SampleRate)
        {
            Ret = -2;
            Verbose(2, "[Warning] '%s' has wrong sampling rate %d. Skipped.\n",
                String_GetChars(& UnitName), DBEntry.Samprate);
            goto SkipSynth;
        }
        RUCE_SoundbankLoadPitchModel(& PMEntry, Session -> Soundbank,
            & UnitName);
        
        //Synthesize unvoiced part.
        int SampleAlign = RUCE_UnvoicedSynth(& UnvoicedWave,
            & Session -> NoteList[i], & DBEntry);
        if(SampleAlign < 1)
        {
            Ret = -2;
            Verbose(2, "[Warning] Failed to synthesize unvoiced part of '%s'."
                " Skipped.\n", String_GetChars(& UnitName));
            goto SkipSynth;
        }
        else
            RCall(InfWave, Add)(Session -> Buffer, UnvoicedWave.Data,
                Sec2Sample(T(i) + N(i).CParamSet.OffsetConsonant) - SampleAlign,
                UnvoicedWave.Size);
        
        //Synthesize voiced part.
        List_HNMContour NoteContour;
        List_DataFrame  NotePhase;
        RCall(List_HNMContour, Ctor)(& NoteContour);
        RCall(List_DataFrame , Ctor)(& NotePhase);
        int ContourAlign = RUCE_VoicedToHNMContour(& NoteContour, & NotePhase,
            F0List, & DBEntry, & PMEntry, Session, i);
        if(ContourAlign < 1)
        {
            Ret = -2;
            Verbose(2, "[Warning] Failed to synthesize voiced part of '%s'."
                " Skipped.\n", String_GetChars(& UnitName));
            RDelete(& NoteContour, & NotePhase);
            goto SkipSynth;
        }
        int VoicedAlign = RUCE_SynthHNMContour(& VoicedWave, & NoiseWave,
            & NoteContour, & NotePhase, F0List, DBEntry.HopSize, Config);
        RDelete(& NoteContour, & NotePhase);
        
    SkipSynth:
        CSVP_PitchModel_Dtor(& PMEntry);
        RUCE_DB_Entry_Dtor(& DBEntry);
        AlignHead = Sec2Sample(T(i));
        Modify(int, Session -> SynthHead) = DestHead;
        i ++;
    }
    
    RDelete(& UnvoicedWave, & VoicedWave, & NoiseWave, & UnitName);
    RFree(F0List);
    
    int N = i - 1;
    if(N >= 0 && N <= Session -> NoteList_Index)
    {
        for(i = 0; i <= N; i ++)
            RUCE_Note_Dtor(& Session -> NoteList[i]);
        Array_RemoveRange(double, Session -> TimeList, 0, N);
        Array_RemoveRange(RUCE_Note, Session -> NoteList, 0, N);
    }
    
    Verbose(3, "SynthHead after this step: %d.\n", Session -> SynthHead);
    Verbose(3, "Wave data submitted at %d.\n", (int)Sec2Sample(Time));
    
    RCall(InfWave, Submit)(Session -> Buffer, Sec2Sample(Time));
    if(Ret < 1)
        return Ret;
    else
        return RCall(InfWave, Dump)(Session -> Buffer, DestBuffer);
}

// Close the RUCE_Session object, but do not destruct it.
// Returns 1 for success, 0 for failure.
int RUCE_SessionSynthFin(RUCE_Session* Session);

// Synthesize a single RUCE_Note object.
// Returns 1 for success, 0 or negative integer for failure.
//   0: Undocumented error.
//  -2: The duration is too long.
int RUCE_SessionSynthNote(RUCE_Session* Session, Real* DestBuffer,
    RUCE_Note* SorcNote);

