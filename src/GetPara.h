#ifndef RUCE_GETPARA_H
#define RUCE_GETPARA_H

#include <RUtil2.h>

RClass(RUCE_RESAMPLER_PARA)
{
    String Input, Output;
    float InvarLeft, InvarRight;
    float LenRequire, FixedLength;
    float Velocity, Volume, Modulation;
    PMatch_Float_Float Freq;
};

#endif // RUCE_GETPARA_H
