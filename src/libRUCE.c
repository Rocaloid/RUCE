#include "libRUCE.h"
#include <CVEDSP2.h>
#include <RUtil2.h>
#include "Common.h"
#include "Config.h"

#define Modify(Type, Name) (*((Type*)(& (Name))))

const RUCE_Version Version = {
    RUCE_VERSION_MAJOR, RUCE_VERSION_MINOR, RUCE_VERSION_REVISION,
    RUCE_VERSION_PATCH, RUCE_VERSION_TAG,
    RUCE_APIMAJOR, RUCE_APIMINOR, RUCE_APIREVISION};

const RUCE_Version* RUCE_GetVersion()
{
    return & Version;
}

RUCE_Note* RUCE_CreateNote()
{
    RUCE_Note* Ret = (RUCE_Note*)malloc(sizeof(RUCE_Note));
    if(! Ret) return NULL;
    Ret -> Duration = 0;
    Ret -> Lyric = (char*)malloc(RUCE_LYRICSIZE);
    if(! Ret -> Lyric)
    {
        free(Ret);
        return NULL;
    }
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
    if(! Ret) return NULL;
    
    Ret -> Buffer = RAlloc_Class(InfWave, 1);
    if(! Ret -> Buffer) return NULL;
    Ret -> FreqMatch = RAlloc_Class(PMatch, 1);
    Ret -> AmplMatch = RAlloc_Class(PMatch, 1);
    Ret -> BreMatch  = RAlloc_Class(PMatch, 1);
    Ret -> GenderMatch = RAlloc_Class(PMatch, 1);
    if(Ret -> FreqMatch == NULL || Ret -> AmplMatch == NULL ||
       Ret -> BreMatch == NULL  || Ret -> GenderMatch == NULL)
        return NULL;
    
    RCall(PMatch, Ctor)(Ret -> FreqMatch);
    RCall(PMatch, Ctor)(Ret -> AmplMatch);
    RCall(PMatch, Ctor)(Ret -> BreMatch);
    RCall(PMatch, Ctor)(Ret -> GenderMatch);
    
    Modify(int, Ret -> SampleRate) = SampleRate;
    Modify(int, Ret -> SynthHead)  = 0;
    Ret -> Soundbank = NULL;
    
    RCall(InfWave, CtorSize)(Ret -> Buffer, BufferSize);
    Array_Ctor(RUCE_Note, Ret -> NoteList);
    Array_Ctor(double, Ret -> TimeList);
    if(Ret -> NoteList == NULL || Ret -> TimeList == NULL) return NULL;
    
    return Ret;
}

int RUCE_DestroySynthSession(RUCE_Session* Session)
{
    RCall(InfWave, Dtor)(Session -> Buffer);
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
        RCall(PMatch, AddPair)((PMatch*)Session -> FreqMatch, Time, Value);
    break;
    case RUCE_EAMPL:
        if(Value < 0) return -3;
        RCall(PMatch, AddPair)((PMatch*)Session -> AmplMatch, Time, Value);
    break;
    case RUCE_EBRE:
        if(Value < 0 || Value > 1) return -3;
        RCall(PMatch, AddPair)((PMatch*)Session -> BreMatch, Time, Value);
    break;
    case RUCE_EGEN:
        if(Value < -1 || Value > 1) return -3;
        RCall(PMatch, AddPair)((PMatch*)Session -> GenderMatch, Time, Value);
    break;
    default:
        return -1;
    }
    return 1;
}

int RUCE_SessionSynthInit(RUCE_Session* Session, double TimeStart)
{
    //Remove notes ends before TimeStart.
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
        printf("%f -> %f\n",
            Session -> TimeList[i],
            Session -> TimeList[i] + Session -> NoteList[i].Duration);
    
    //Check validity of TimeStart.
    if(TopOf(Session -> TimeList) + TopOf(Session -> NoteList).Duration
     < TimeStart)
        return -1;
    
    Modify(int, Session -> SynthHead) = TimeStart * Session -> SampleRate;
    RCall(InfWave, Relocate)((InfWave*)Session -> Buffer, Session -> SynthHead);
    
    return 1;
}

// Synthesize to a certain time point(Time) and dump audio into DestBuffer.
// Returns number of samples dumped into DestBuffer for success, 0 or negative
//   integer for failure.
//   0: Undocumented error.
//  -1: Invalid TimeStart.
//  -2: Synthesis succeeded, but one or more notes are not pronounced.
int RUCE_SessionSynthStep(RUCE_Session* Session, RUCE_FPTYPE* DestBuffer,
    double Time);

// Close the RUCE_Session object, but do not destruct it.
// Returns 1 for success, 0 for failure.
int RUCE_SessionSynthFin(RUCE_Session* Session);

// Synthesize a single RUCE_Note object.
// Returns 1 for success, 0 or negative integer for failure.
//   0: Undocumented error.
//  -1: Invalid TimeStart.
//  -2: The duration is too long.
int RUCE_SessionSynthNote(RUCE_Session* Session, RUCE_FPTYPE* DestBuffer,
    RUCE_Note* SorcNote);

