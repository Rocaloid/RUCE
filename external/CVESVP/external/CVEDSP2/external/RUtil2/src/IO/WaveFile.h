#ifndef RUTIL2_WAVEFILE_H
#define RUTIL2_WAVEFILE_H

#include "File.h"
#include "../Core/OO.h"

RClass(PCMHeader)
{
    //Required while writing
    short int Channel;
    int SamplePerSecond;
    int BytePerSecond;
    short int BlockAlign;
    short int BitPerSample;

    //Temporary data used while reading
    int DataSize;
    int DataPosition;
    int DataNum;
    int BytePerSample;
};

RClass(WaveFile)
{
    RInherit(File);
    PCMHeader Header;
    int WriteCounter;
    int WritePosition;
};

int WaveFile_Open(WaveFile* This, String* Sorc);
int WaveFile_Save(WaveFile* This, String* Dest);

void WaveFile_PrintInfo(WaveFile* This);

void WaveFile_FinishWrite(WaveFile* This);
void WaveFile_Close(WaveFile* This);

int WaveFile_FetchAll(WaveFile* This, char* Dest);
int WaveFile_FetchAllFloat(WaveFile* This, float* Dest);
int WaveFile_FetchAllFloatStereo(WaveFile* This, float* DestL, float* DestR);
float WaveFile_FetchAt(WaveFile* This, int Position);
float WaveFile_FetchLAt(WaveFile* This, int Position);
float WaveFile_FetchRAt(WaveFile* This, int Position);

void WaveFile_WriteAll(WaveFile* This, char* Sorc, int Size);
void WaveFile_WriteAllFloat(WaveFile* This, float* Sorc, int Size);
void WaveFile_WriteAllFloatStereo(WaveFile* This, float* SorcL, float* SorcR,
    int Size);
void WaveFile_Write(WaveFile* This, float Data);
void WaveFile_WriteStereo(WaveFile* This, float DataL, float DataR);

#endif //RUTIL2_WAVEFILE_H

