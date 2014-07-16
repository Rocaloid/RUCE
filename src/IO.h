#ifndef RUCE_IO_H
#define RUCE_IO_H

#include <RUtil2.h>

RClass(RUCE_DB_Frame)
{
    RInherit(RObject);
    
    //Public
    int Position;
    Array_Define(float, Freq);
    Array_Define(float, Ampl);
    Array_Define(float, Phse);
    
    float* Noiz;
};

RClass(RUCE_DB_Entry)
{
    RInherit(RObject);
    
    //Public
    int HopSize;
    int NoizSize;
    Array_Define(DB_Frame, FrameList);
    Array_Define(int, PulseList);
    
    int WaveSize;
    float* Wave;
    
    int VOT;
    int InvarLeft;
    int InvarRight;
};

#endif

