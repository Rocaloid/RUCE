#include "WaveFile.h"
#include <string.h>
#include <stdio.h>

#define WStream MyBase(File)
#define Header (This -> Header)

RCtor(WaveFile)
{
    This -> WriteCounter = 0;
    This -> WritePosition = 0;
    Header.DataNum = 0;
    Header.DataSize = 0;
    File_Ctor(MyBase(File));
    RInit(WaveFile);
}

RDtor(WaveFile)
{
    File_Dtor(MyBase(File));
}

int WaveFile_Open(WaveFile* This, String* Sorc)
{
    int OpenStatus = File_Open(WStream, Sorc, READONLY);
    if(! OpenStatus)
        return 0;

    char RIFFChars[5];
    char WAVEChars[5];
    char FACTChars[5];
    RIFFChars[4] = 0;
    WAVEChars[4] = 0;
    FACTChars[4] = 0;

    File_Read_Buffer(WStream, RIFFChars, 4); //RIFF
    File_Read_Int(WStream); //Size
    File_Read_Buffer(WStream, WAVEChars, 4); //WAVE

    if(strcmp(RIFFChars, "RIFF"))
        return 0;
    if(strcmp(WAVEChars, "WAVE"))
        return 0;

    File_Read_Int32(WStream); //fmt
    int fmtSize            = File_Read_Int32(WStream);
    File_Read_Int16(WStream); //FormatTag
    Header.Channel         = File_Read_Int16(WStream);
    Header.SamplePerSecond = File_Read_Int32(WStream);
    Header.BytePerSecond   = File_Read_Int32(WStream);
    Header.BlockAlign      = File_Read_Int16(WStream);
    Header.BitPerSample    = File_Read_Int16(WStream);

    if(fmtSize == 16)
    {
        //No extra info
    }else
        File_Read_Int16(WStream); //Extra info
    File_Read_Buffer(WStream, FACTChars, 4);

    if(! strcmp(FACTChars, "fact"))
    {
        int factSize = File_Read_Int32(WStream);
        File_SetPosition(WStream, File_GetPosition(WStream) + factSize + 4);
    }

    Header.DataSize = File_Read_Int32(WStream);
    Header.DataPosition = File_GetPosition(WStream);
    Header.BytePerSample = Header.BitPerSample / 8;
    Header.DataNum = Header.DataSize / Header.BytePerSample / Header.Channel;

    return 1;
}

int WaveFile_Save(WaveFile* This, String* Dest)
{
    int SaveStatus = File_Open(WStream, Dest, CREATE);
    if(! SaveStatus)
        return 0;

    File_Write_Buffer(WStream, "RIFF", 4);
    File_Write_Int32(WStream, 0); //Size = 0 by default
    File_Write_Buffer(WStream, "WAVE", 4);

    File_Write_Buffer(WStream, "fmt ", 4);
    File_Write_Int32(WStream, 16); //No extra
    File_Write_Int16(WStream, 1); //FormatTag = 0x0001
    File_Write_Int16(WStream, Header.Channel);
    File_Write_Int32(WStream, Header.SamplePerSecond);
    File_Write_Int32(WStream, Header.BytePerSecond);
    File_Write_Int16(WStream, Header.BlockAlign);
    File_Write_Int16(WStream, Header.BitPerSample);

    File_Write_Buffer(WStream, "data", 4);
    File_Write_Int32(WStream, 0); //DataSize = 0 by default

    This -> WritePosition = File_GetPosition(WStream);
    This -> WriteCounter = 0;
    Header.BytePerSample = Header.BitPerSample / 8;
    return 1;
}

void WaveFile_PrintInfo(WaveFile* This)
{
    printf("Number of channels: %d\n", Header.Channel);
    printf("Sample rate: %d\n", Header.SamplePerSecond);
    printf("Byte rate: %d\n", Header.BytePerSecond);
    printf("Bit per sample: %d\n", Header.BitPerSample);
    printf("Data size: %d\n", Header.DataSize);
    printf("Sample num: %d\n", Header.DataNum);
}

void WaveFile_FinishWrite(WaveFile* This)
{
    int FileSize = File_GetPosition(WStream);

    File_SetPosition(WStream, 4); //Size
    File_Write_Int32(WStream, FileSize - 8);
    int DataSize = 
        This -> WriteCounter * Header.Channel * Header.BitPerSample / 8;

    File_SetPosition(WStream, This -> WritePosition - 4); //dataSize
    File_Write_Int32(WStream, DataSize);
    File_Flush(WStream);

    File_Close(WStream);
}

void WaveFile_Close(WaveFile* This)
{
    File_Close(WStream);
}

int WaveFile_FetchAll(WaveFile* This, char* Dest)
{
    File_SetPosition(WStream, Header.DataPosition);
    File_Read_Buffer(WStream, Dest, Header.DataSize);
    return Header.DataSize;
}

int WaveFile_FetchAllFloat(WaveFile* This, float* Dest)
{
    int Ret = 0;
    if(Header.BitPerSample == 8)
    {
        int i;
        char* tmp = (char*)malloc(Header.DataSize);
        WaveFile_FetchAll(This, tmp);
        Ret = Header.DataSize;
        for(i = 0; i < Ret; i ++)
            Dest[i] = (float)tmp[i] / 127.0;
        free(tmp);
    }else if(Header.BitPerSample == 16)
    {
        int i;
        short int* tmp = (short int*)malloc(Header.DataSize);
        WaveFile_FetchAll(This, (char*)tmp);
        Ret = Header.DataSize / 2;
        for(i = 0; i < Ret; i ++)
            Dest[i] = (float)tmp[i] / 32767.0;
        free(tmp);
    }
    return Ret;
}

int WaveFile_FetchAllFloatStereo(WaveFile* This, float* DestL, float* DestR)
{
    int Ret = 0;
    if(Header.BitPerSample == 8)
    {
        int i;
        char* tmp = (char*)malloc(Header.DataSize);
        WaveFile_FetchAll(This, tmp);
        Ret = Header.DataSize / 2;
        for(i = 0; i < Ret; i ++)
        {
            DestL[i] = (float)tmp[i * 2] / 127.0;
            DestR[i] = (float)tmp[i * 2 + 1] / 127.0;
        }
        free(tmp);
    }else if(Header.BitPerSample == 16)
    {
        int i;
        short int* tmp = (short int*)malloc(Header.DataSize);
        WaveFile_FetchAll(This, (char*)tmp);
        Ret = Header.DataSize / 4;
        for(i = 0; i < Ret; i ++)
        {
            DestL[i] = (float)tmp[i * 2] / 32767.0;
            DestR[i] = (float)tmp[i * 2 + 1] / 32767.0;
        }
        free(tmp);
    }
    return Ret;
}

float WaveFile_FetchAt(WaveFile* This, int Position)
{
    File_SetPosition(WStream, Header.DataPosition +
        Position * Header.BytePerSample);
    if(Header.BitPerSample == 8)
        return (float)File_Read_Char(WStream) / 127.0;
    if(Header.BitPerSample == 16)
        return (float)File_Read_Short(WStream) / 32767.0;
    return 0;
}

float WaveFile_FetchLAt(WaveFile* This, int Position)
{
    File_SetPosition(WStream, Header.DataPosition +
        Position * Header.BytePerSample * 2);
    if(Header.BitPerSample == 8)
        return (float)File_Read_Char(WStream) / 127.0;
    if(Header.BitPerSample == 16)
        return (float)File_Read_Short(WStream) / 32767.0;
    return 0;
}

float WaveFile_FetchRAt(WaveFile* This, int Position)
{
    File_SetPosition(WStream, Header.DataPosition +
        Position * Header.BytePerSample * 2 + Header.BytePerSample);
    if(Header.BitPerSample == 8)
        return (float)File_Read_Char(WStream) / 127.0;
    if(Header.BitPerSample == 16)
        return (float)File_Read_Short(WStream) / 32767.0;
    return 0;
}

void WaveFile_WriteAll(WaveFile* This, char* Sorc, int Size)
{
    File_Write_Buffer(WStream, Sorc, Size);
    This -> WriteCounter = Size / Header.BytePerSample;
}

void WaveFile_WriteAllFloat(WaveFile* This, float* Sorc, int Size)
{
    if(Header.BitPerSample == 8)
    {
        char* tmp = (char*)malloc(Size);
        int i;
        for(i = 0; i < Size; i ++)
            tmp[i] = (char)(Sorc[i] * 127);
        WaveFile_WriteAll(This, tmp, Size);
        free(tmp);
    }else if(Header.BitPerSample == 16)
    {
        short int* tmp = (short int*)malloc(Size * 2);
        int i;
        for(i = 0; i < Size; i ++)
            tmp[i] = (short int)(Sorc[i] * 32767);
        WaveFile_WriteAll(This, (char*)tmp, Size * 2);
        free(tmp);
    }
}

void WaveFile_WriteAllFloatStereo(WaveFile* This, float* SorcL, float* SorcR,
    int Size)
{
    if(Header.BitPerSample == 8)
    {
        char* tmp = (char*)malloc(Size * 2);
        int i;
        for(i = 0; i < Size; i ++)
        {
            tmp[i * 2 + 0] = (char)(SorcL[i] * 127);
            tmp[i * 2 + 1] = (char)(SorcR[i] * 127);
        }
        WaveFile_WriteAll(This, tmp, Size * 2);
        free(tmp);
    }else if(Header.BitPerSample == 16)
    {
        short int* tmp = (short int*)malloc(Size * 4);
        int i;
        for(i = 0; i < Size; i ++)
        {
            tmp[i * 2 + 0] = (short int)(SorcL[i] * 32767);
            tmp[i * 2 + 1] = (short int)(SorcR[i] * 32767);
        }
        WaveFile_WriteAll(This, (char*)tmp, Size * 4);
        free(tmp);
    }
}

void WaveFile_Write(WaveFile* This, float Data)
{
    if(Header.BitPerSample == 8)
        File_Write_Char(WStream, (char)(Data * 127));
    if(Header.BitPerSample == 16)
        File_Write_Short(WStream, (short int)(Data * 32767));
    This -> WriteCounter ++;
}

void WaveFile_WriteStereo(WaveFile* This, float DataL, float DataR)
{
    if(Header.BitPerSample == 8)
    {
        File_Write_Char(WStream, (char)(DataL * 127));
        File_Write_Char(WStream, (char)(DataR * 127));
    }
    if(Header.BitPerSample == 16)
    {
        File_Write_Short(WStream, (short int)(DataL * 32767));
        File_Write_Short(WStream, (short int)(DataR * 32767));
    }
    This -> WriteCounter ++;
}

