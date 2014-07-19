#ifndef RUCE_IO_H
#define RUCE_IO_H

#include <CVESVP.h>
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
    
    /* from rudb */
    int HopSize;
    int NoizSize;
    Array_Define(RUCE_DB_Frame, FrameList);
    Array_Define(int, PulseList);
    
    /* from wave */
    int WaveSize;
    float* Wave;
    
    /* from roto */
    int VOT;
    int InvarLeft;
    int InvarRight;
};

void RUCE_LoadPitchModel(CSVP_PitchModel* Dest, String* Sorc, String* Path);
int RUCE_DB_ReadEntry(RUCE_DB_Entry* Dest, String* Sorc, String* Path, String* RotoPath);

#endif

