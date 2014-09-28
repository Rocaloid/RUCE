#ifndef RUCE_DATA_H
#define RUCE_DATA_H

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
    
    int HopSize;
    int NoizSize;
    Array_Define(RUCE_DB_Frame, FrameList);
    
    int WaveSize;
    int Samprate;
    float* Wave;
    
    float VOT; //Voice Onset Time
    float SOT; //Sinusoidal Onset Time
    float InvarLeft;
    float InvarRight;
};

void RUCE_LoadPitchModel(CSVP_PitchModel* Dest, String* Sorc, String* Path);

//Deprecated
int RUCE_DB_LoadEntry(RUCE_DB_Entry* Dest, String* Sorc, String* Path);

int RUCE_DB_LoadExternWave(RUCE_DB_Entry* Dest, String* Sorc, String* Path);
int RUCE_DB_RUDBWriteEntry(RUCE_DB_Entry* Sorc, String* Dest, String* Path);
int RUCE_DB_WaveWriteEntry(RUCE_DB_Entry* Sorc, String* Dest, String* Path);
void RUCE_DB_PrintEntry(RUCE_DB_Entry* Sorc);

#define _JSON_SafeRead(Name) do {\
    cJSON* Name = cJSON_GetObjectItem(Entry, _S(Name)); \
    if(Name) \
    { \
        Dest -> Name = Name -> valuedouble; \
    } \
    } while(0)

#endif

