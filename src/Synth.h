#ifndef RUCE_SYNTH_H
#define RUCE_SYNTH_H

#include "IO.h"
#include <CVESVP.h>
#include <RUtil2.h>
#include "Common.h"

RClass(RUCE_UnitParam)
{
    //Public
    String Input, Output;
    
    float InvarLeft, InvarRight;
    float LenRequire, FixedLength;
    float Velocity, Volume, Modulation;
    
    struct
    {
        float Breathness;
        float Gender;
        float DeltaDuration;
        float PhaseSync;
        
        float CLoudness;
        float CStretch;
        float COffset;
        float DeltaSeg1;
        float DeltaSeg2;
        float SmoothenRate;
        float SmoothenRadius;
        
        int   Verbose;
    } FlagPara;
    
    PMatch_Float_Float Freq;
};

int RUCE_SynthUnit(_Wave* Dest, _Wave* Sorc, RUCE_DB_Entry* SorcDB,
    CSVP_PitchModel* PM, RUCE_UnitParam* Para);

#endif

