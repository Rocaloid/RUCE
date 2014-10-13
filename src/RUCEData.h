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
    
    int    HopSize;
    int    NoizSize;
    Array_Define(RUCE_DB_Frame, FrameList);
    
    int    WaveSize;
    int    Samprate;
    float* ApWave;
    
    float  VOT; //Voice Onset Time
    float  SOT; //Sinusoidal Onset Time
    float  InvarLeft;
    float  InvarRight;
};

int RUCE_PitchModelFromJSONEntries(CSVP_PitchModel* Dest, void* Entries,
    String* Name);

#define RUDB_VERSION 4
int  RUCE_RUDB_Load(RUCE_DB_Entry* Dest, String* Path);
int  RUCE_RUDB_Save(RUCE_DB_Entry* Sorc, String* Path);
void RUCE_RUDB_Print(RUCE_DB_Entry* Sorc);

#define _JSON_SafeRead(Name) do {\
    cJSON* Name = cJSON_GetObjectItem(Entry, _S(Name)); \
    if(Name) \
    { \
        Dest -> Name = Name -> valuedouble; \
    } \
    } while(0)

#endif

