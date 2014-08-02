#ifndef RUCE_GETPARA_H
#define RUCE_GETPARA_H

#include <RUtil2.h>

RClass(RUCE_ResamplerPara)
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
        int   Verbose;
    } FlagPara;
    
    PMatch_Float_Float Freq;
};

int RUCE_ParsePara(RUCE_ResamplerPara* Dest, int argc, char** argv);

#endif // RUCE_GETPARA_H

