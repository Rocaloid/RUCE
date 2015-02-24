#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Tune.h"
#include "../Core/OO.h"

static const bool SemitoneTable[12] =
{
    false, true, false, true, false, false, true,
    false, true, false, true, false  
};

static const char PNTable[12][3][4] =
{
    {"C"}, {"C#", "Db", "C#"}, 
    {"D"}, {"D#", "Eb", "Eb"}, 
    {"E"}, {"F"}, 
    {"F#", "Gb", "F#"}, {"G"}, 
    {"G#", "Ab", "Ab"}, {"A"}, 
    {"A#", "Bb", "Bb"}, {"B"}
};

static enum Tune_PitchNameMode PNMode = Tune_AlwaysRising;

void Tune_SetPitchNameMode(enum Tune_PitchNameMode m)
{
    PNMode = m;
}

#if 0
#include "_Tune.rc"
#endif

#define _RTAddress "Misc/_Tune.rc"
#define _ClassName
#define _Attr 1

#define _T1 Float
#include "../Core/RTemplate.h"

#define _T1 Double
#include "../Core/RTemplate.h"

#define _T1 Int
#include "../Core/RTemplate.h"

