#ifndef RUCE_GETPARA_H
#define RUCE_GETPARA_H

#include <RUtil2.h>

RClass(RUCE_ResamplerPara)
{
    String Input, Output;
    float InvarLeft, InvarRight;
    float LenRequire, FixedLength;
    float Velocity, Volume, Modulation;
    PMatch_Float_Float Freq;
};

int RUCE_ParsePara(RUCE_ResamplerPara* Dest, int argc, char** argv);

#endif // RUCE_GETPARA_H

