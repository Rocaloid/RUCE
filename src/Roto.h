#ifndef RUCE_ROTO_H
#define RUCE_ROTO_H

#include <RUtil2.h>

RClass(RUCE_Roto_Entry)
{
    RInherit(RObject);
    
    //Public
    String Name;
    float VOT;
    float InvarLeft;
    float InvarRight;
    float Overlap;
};

RClass(RUCE_Roto)
{
    RInherit(RObject);
    
    //Private
    void* Ptr;
};

int  RUCE_Roto_CtorLoad(RUCE_Roto* This, String* Path);
int  RUCE_Roto_Write(RUCE_Roto* This, String* Path);

int  RUCE_Roto_GetEntry(RUCE_Roto* This, RUCE_Roto_Entry* Dest, String* Name);
int  RUCE_Roto_GetEntryNum(RUCE_Roto* This);
int  RUCE_Roto_GetEntryByIndex(RUCE_Roto* This, RUCE_Roto_Entry* Dest,
    int Index);
int  RUCE_Roto_SetEntry(RUCE_Roto* This, RUCE_Roto_Entry* Sorc);

#endif

