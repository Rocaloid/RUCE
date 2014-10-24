#ifndef RUCE_TIMING_H
#define RUCE_TIMING_H

#include "Soundbank.h"
#include "SessionConfig.h"
#include "Common.h"
#include "libRUCE.h"

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

typedef struct
{
    double T0, TV, T1, T2, TD;
} Segmentation;

Segmentation MatchUnitToNote(PMatch* Dest, RUCE_DB_Entry* SorcDB,
    RUCE_Session* Session, int NoteIndex);

#define Sample2Sec(x) ((double)(x) / SampleRate)
#define Sec2Sample(x) (SampleRate * (x))

#endif

