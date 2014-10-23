#ifndef RUCE_SYNTH_H
#define RUCE_SYNTH_H

#include "Soundbank.h"
#include "SessionConfig.h"
#include "Common.h"
#include "libRUCE.h"

int RUCE_UnvoicedSynth(Wave* Dest, RUCE_Note* SorcNote, RUCE_DB_Entry* SorcDB);

int RUCE_VoicedToHNMContour(List_HNMContour* Dest, List_DataFrame* DestPhse,
    Real* DestF0, RUCE_DB_Entry* SorcDB, CSVP_PitchModel* SorcPM,
    RUCE_Session* Session, int NoteIndex);
int RUCE_SynthHNMContour(Wave* DestHmnc, Wave* DestNoiz, List_HNMContour* Sorc,
    List_DataFrame* SorcPhse, Real* SorcF0, int HopSize,
    RUCE_SessionConfig* SorcConfig);

#endif

