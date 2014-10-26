#ifndef RUCE_LIBRUCE_H
#define RUCE_LIBRUCE_H

/*
  libRUCE API 0.1.1
  This header is designed to be included without direct dependency on RUtil2.
  
  2014.9.9-0.0.0
    First draft.
  
  2014.9.10-0.0.1
    1. const RUCE_Version* RUCE_GetVersion();
    2. Remove vibrato parameters from RUCE_Note.
    3. Constant SampleRate and SynthHead.
    4. SynthInit, SynthStep instead of SynthPre, SynthTo.
  
  2014.10.8-0.0.2
    RUCE_SoundBank --> RUCE_Soundbank.
    Add more descriptions to RUCE_SessionSynthStep.
  
  2014.10.15-0.1.0
    Add internal session configuration to RUCE_Session.
    Add verbose control.
  
  2014.10.24-0.1.1
    Remove const qualifiers from declarations of RUCE_Session.SampleRate and
      RUCE_Session.SynthHead. (see ISO/IEC 9899:TC3, 6.7.3.5)
    Remove single note synthesis.
  
  2014.10.26-0.1.2
    Add const qualifier to char* Directory in RUCE_CreateLoadSoundbank.
*/

//-----Macros-----

#define RUCE_FPTYPE float

#define RUCE_EFREQ   0
#define RUCE_EAMPL   1
#define RUCE_EBRE    2
#define RUCE_EGEN    3

#define RUCE_LYRICSIZE 16

// API version of this header.
#define RUCE_APIMAJOR    0
#define RUCE_APIMINOR    1
#define RUCE_APIREVISION 2

//-----Structures-----
/*
Modifiable          You can read/modify the object, as well as the members of
                      the object(if the object has a struct type).
Non-Modifiable      Read only.
*/

// libRUCE & API version of the library.
typedef struct
{
    int Major;
    int Minor;
    int Revision;
    int Patch;
    char* Tag;
    int APIMajor;
    int APIMinor;
    int APIRevision;
} RUCE_Version;

// Internal structure of RUCE_Soundbank is so complicated that is hidden from
//   users.
typedef void RUCE_Soundbank;

typedef struct
{
    //Essential Parameters
    double Duration;     //Modifiable
    char*  Lyric;        //Modifiable
    
    //Context-Related Parameter Set
    struct //Modifiable
    {
        double DeltaOnset;
        double DeltaDuration;
        double DurFinal;
        double DurInitial;
        double DurConsonant;
        double OffsetConsonant;
        double AmplConsonant;
    } CParamSet;
    
    //Expressiveness Paramters
    //Modifiable
    double PhaseSync;
} RUCE_Note;

typedef struct
{
    //Public
    int SampleRate; //Non-Modifiable
    int SynthHead;  //Non-Modifiable
    
    //Private
    RUCE_Soundbank* Soundbank;
    
    void* Buffer;
    RUCE_Note* NoteList;
    int        NoteList_Index;
    int        NoteList_Size;
    double*    TimeList;
    int        TimeList_Index;
    int        TimeList_Size;
    void* FreqMatch;
    void* AmplMatch;
    void* BreMatch;
    void* GenderMatch;
    
    void* Config;
} RUCE_Session;

//-----Functions-----

// Get the version of libRUCE.
// You do not have to destruct RUCE_Version.
const RUCE_Version* RUCE_GetVersion();

// Show diagnostic log through stderr.
// Verbose Level:
// <= 0: No verbose
//    1: Errors only
//    2: Warnings
// >= 3: Other information
void RUCE_SetVerboseLevel(int Level);

// Allocate and construct a RUCE_Note object.
// Return NULL if failed.
// By default a RUCE_LYRICSIZE-sized memory region is allocated to Lyric and is
//   initiated with zeros.
// For longer content you need realloc to set Lyric:
//   RUCE_Note* Note = RUCE_CreateNote();
//   Note -> Lyric = (char*)realloc(50);
//   strcpy(Note -> Lyric, "...");
RUCE_Note* RUCE_CreateNote();

// Destruct and free a RUCE_Note object.
// Return 1 for success, 0 for failure.
int RUCE_DestroyNote(RUCE_Note* Note);

// Allocate and construct a RUCE_Soundbank object.
// Example: RUCE_Soundbank* Singer = RUCE_CreateLoadSoundbank("/tmp/bank/");
// Return NULL if failed.
RUCE_Soundbank* RUCE_CreateLoadSoundbank(const char* Directory);

// Destruct and free a RUCE_Soundbank object.
// Return 1 for success, 0 for failure.
int RUCE_DestroySoundbank(RUCE_Soundbank* Soundbank);

// Allocate and construct a RUCE_Session object.
// BufferSize is the size of an internal buffer for the session. The longest
//   note that the session is able to produce is determined by half of
//   BufferSize.
// Example: RUCE_Sessoin* Track1 = RUCE_CreateSynthSession(44100, 44100 * 60);
// Return NULL if failed.
RUCE_Session* RUCE_CreateSynthSession(int SampleRate, int BufferSize);

// Destruct and free a RUCE_Session object.
// Return 1 for sucess, 0 for failure.
int RUCE_DestroySynthSession(RUCE_Session* Session);

// Bind a RUCE_Soundbank object to a RUCE_Session object.
// Sorc should not be destructed until synthesis is finished.
void RUCE_SessionSetSoundbank(RUCE_Session* Session, RUCE_Soundbank* Sorc);

// Append a RUCE_Note object to a RUCE_Session object.
// SorcNote can be destructed since it has been appended.
// Unit for Time: second
// Returns 1 for success, 0 or negative integer for failure.
//   0: Undocumented error.
//  -1: The duration is too long.
//  -2: Invalid time.
int RUCE_SessionAppendNote(RUCE_Session* Session, RUCE_Note* SorcNote,
    double Time);

// Append a control event to a RUCE_Session object.
// Type: see Macros, RUCE_E*
// Units
//   RUCE_EFREQ    Hertz
//   RUCE_EAMPL    Ratio
//   RUCE_EBRE     Ratio(0 to 1)
//   RUCE_EGEN     Ratio(0 to 1)
// Unit for Time: second
// Returns 1 for success, 0 or negative integer for failure.
//   0: Undocumented error.
//  -1: Invalid event type.
//  -2: Invalid time.
//  -3: Invalid value.
int RUCE_SessionAppendEvent(RUCE_Session* Session, int Type, double Time,
    double Value);

// Initiate the synthesis.
// TimeStart: the time at where the synthesis starts, in second.
// Returns 1 for success, 0 or negative integer for failure.
//   0: Undocumented error.
//  -1: Invalid TimeStart.
int RUCE_SessionSynthInit(RUCE_Session* Session, double TimeStart);

// Synthesize to a certain time point(Time) and dump audio into DestBuffer.
// Returns number of samples dumped into DestBuffer for success, 0 or negative
//   integer for failure.
//   0: Undocumented error.
//  -1: Invalid Time.
//  -2: Synthesis succeeded, but one or more notes are not pronounced. Run
//      RUCE_SessionSynthStep again to get the audio output.
int RUCE_SessionSynthStep(RUCE_Session* Session, RUCE_FPTYPE* DestBuffer,
    double Time);

// Close the RUCE_Session object, but do not destruct it.
// Returns 1 for success, 0 for failure.
int RUCE_SessionSynthFin(RUCE_Session* Session);

#endif

