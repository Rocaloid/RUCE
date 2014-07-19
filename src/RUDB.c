#include "RUDB.h"
#include <stdio.h>
#include <RUtil2.h>

static const int RUDB_Header = 0x42445552;
static const int _RUDB_Version_ = RUDB_VERSION;

int RUCE_RUDB_Load(RUCE_DB_Entry* Dest, String* Path)
{
    File Sorc;
    File_Ctor(& Sorc);
    if(! File_Open(& Sorc, Path, READONLY))
        return -1;
    
    int ReverseEndian = 0;
    
    int64_t FLen = File_GetLength(& Sorc);
    if(FLen < 48)
        return -1;
    char CBuffer[8];
    uint32_t Header[4];
    uint64_t DataSize;
    File_Read_Buffer(& Sorc, Header, 12);
    if(Header[0] != RUDB_Header)
    {
        Endian_Switch_Array_UInt32(Header, 4);
        ReverseEndian = 1;
    }
    if(Header[0] != RUDB_Header)
        return -2;
    if(Header[2] > RUDB_VERSION)
        return -3;
    File_Read_Buffer(& Sorc, CBuffer, 4);
    if(strncmp(CBuffer, "DATA", 4))
        return -4;
    
    File_Read_Buffer(& Sorc, & DataSize, 8);
    
    if(DataSize < 8)
        return -5;
    
    char* Data = malloc(DataSize);
    File_Read_Buffer(& Sorc, Data, DataSize);
    
    if(Header[1] != CRC32Sum(Data, DataSize, 0))
        return -6;
    
    memcpy(& (Dest -> HopSize), Data, 8);
    Data += 8;
    if(ReverseEndian)
        Endian_Switch_Array_Int32(& (Dest -> HopSize), 2);
    
    while(Data != Data + DataSize)
    {
        if(! strncmp(Data, "FRME", 4))
        {
            Data += 4;
            memcpy(Header, Data, 4);
            Data += 4;
            if(ReverseEndian)
                Endian_Switch_UInt32(Header);
            int OldSize = Dest -> FrameList_Index + 1;
            Array_Resize(RUCE_DB_Frame, 
                         Dest -> FrameList, 
                         OldSize + Header[0]);
            for(int i = 0 + OldSize; i < Header[0] + OldSize; ++i)
            {
                RUCE_DB_Frame_Ctor(& (Dest -> FrameList[i]));
                if(strncmp(Data, "FRMB", 4))
                    return -255;
                Data += 4;
                memcpy(& (Dest -> FrameList[i].Position), Data, 4);
                Data += 4;
                int Cnt;
                memcpy(& Cnt, Data, 4);
                Data += 4;
                if(ReverseEndian)
                {
                    Endian_Switch_Int32(& (Dest -> FrameList[i].Position));
                    Endian_Switch_Int32(& Cnt);
                }
                Array_Resize(float, Dest -> FrameList[i].Freq, Cnt);
                Array_Resize(float, Dest -> FrameList[i].Ampl, Cnt);
                Array_Resize(float, Dest -> FrameList[i].Phse, Cnt);
                Dest -> FrameList[i].Freq_Index = Cnt - 1;
                Dest -> FrameList[i].Ampl_Index = Cnt - 1;
                Dest -> FrameList[i].Phse_Index = Cnt - 1;
                if(Dest -> FrameList[i].Noiz)
                    free(Dest -> FrameList[i].Noiz);
                Dest -> FrameList[i].Noiz = RAlloc_Float(Dest -> NoizSize);
                memcpy(Dest -> FrameList[i].Freq, Data, Cnt * 4);
                Data += Cnt * 4;
                memcpy(Dest -> FrameList[i].Ampl, Data, Cnt * 4);
                Data += Cnt * 4;
                memcpy(Dest -> FrameList[i].Phse, Data, Cnt * 4);
                Data += Cnt * 4;
                memcpy(Dest -> FrameList[i].Noiz, Data, Dest -> NoizSize * 4);
                Data += Dest -> NoizSize * 4;
                if(ReverseEndian)
                {
                    Endian_Switch_Array_Float(Dest -> FrameList[i].Freq, Cnt);
                    Endian_Switch_Array_Float(Dest -> FrameList[i].Ampl, Cnt);
                    Endian_Switch_Array_Float(Dest -> FrameList[i].Phse, Cnt);
                    Endian_Switch_Array_Float(Dest -> FrameList[i].Noiz, 
                                              Dest -> NoizSize);
                }
            }
        }
        else if(! strncmp(Data, "PULS", 4))
        {
            Data += 4;
            memcpy(Header, Data, 4);
            Data += 4;
            if(ReverseEndian)
                Endian_Switch_UInt32(Header);
            int OldSize = Dest -> PulseList_Index + 1;
            Array_Resize(int, Dest -> PulseList, Header[0] + OldSize);
            memcpy(& (Dest -> PulseList[OldSize]), Data, Header[0] * 4);
            Data += Header[0] * 4;
            if(ReverseEndian)
                Endian_Switch_Array_Int32(Dest -> PulseList, Header[0]);
        }
        else if(! strncmp(Data, "EOL3", 4))
        {
            Data += 4;
            break;
        }
        else
        {
            Data += 4;
            return -23333;
        }
    }
    
    free(Data - DataSize);
    File_Close(& Sorc);
    File_Dtor(& Sorc);
    return 1;
} 

int RUCE_RUDB_Save(RUCE_DB_Entry* Sorc, String* Path)
{
    File Dest;
    File_Ctor(& Dest);
    if(! File_Open(& Dest, Path, WRITEONLY))
        return -1;
    
    uint64_t Size = 0;
    char* Data, * Curr;
    
    UInt FrameCnt = Sorc -> FrameList_Index + 1;
    UInt PulseCnt = Sorc -> PulseList_Index + 1;
    
    /**
     * Calculate data size.
     * HopSize, NoizSize, FRME, FrameCount, PULS, PulseCount, EOL3. (7 * 4)
     * 3 * 4 bytes fixed + 4 * NoizSize + 12 * Count per frame.
     * 4 * PulseCount.
     */
    Size += 28 + FrameCnt * (12 + 4 * Sorc -> NoizSize) + PulseCnt * 4;
    for(int i = 0; i < FrameCnt; ++ i)
        Size += 12 * (Sorc -> FrameList[i].Freq_Index + 1);
    
    Data = malloc(Size);
    Curr = Data;
    
    memcpy(Curr, & (Sorc -> HopSize), 8);
    Curr += 8;
    
    if(FrameCnt)
    {
        memcpy(Curr, "FRME", 4);
        Curr += 4;
        memcpy(Curr, & (FrameCnt), 4);
        Curr += 4;
        for(int i = 0; i < FrameCnt; ++i)
        {
            int Cnt = Sorc -> FrameList[i].Freq_Index + 1;
            memcpy(Curr, "FRMB", 4);
            Curr += 4;
            memcpy(Curr, & (Sorc -> FrameList[i].Position), 4);
            Curr += 4;
            memcpy(Curr, & Cnt, 4);
            Curr += 4;
            memcpy(Curr, Sorc -> FrameList[i].Freq, 4 * Cnt);
            Curr += 4 * Cnt;
            memcpy(Curr, Sorc -> FrameList[i].Ampl, 4 * Cnt);
            Curr += 4 * Cnt;
            memcpy(Curr, Sorc -> FrameList[i].Phse, 4 * Cnt);
            Curr += 4 * Cnt;
            memcpy(Curr, Sorc -> FrameList[i].Noiz, 4 * Sorc -> NoizSize);
            Curr += 4 * Sorc -> NoizSize;
        }
    }
    
    if(PulseCnt)
    {
        memcpy(Curr, "PULS", 4);
        Curr += 4;
        memcpy(Curr, & (PulseCnt), 4);
        Curr += 4;
        memcpy(Curr, Sorc -> PulseList, 4 * PulseCnt);
        Curr += 4 * PulseCnt;
    }
    
    memcpy(Curr, "EOL3", 4);
    
    int CRC = CRC32Sum(Data, Size, 0);
    File_Write_Buffer(& Dest, (char *)(& RUDB_Header), 4);
    File_Write_Buffer(& Dest, (char *)(& CRC), 4);
    File_Write_Buffer(& Dest, (char *)(& _RUDB_Version_), 4);
    File_Write_Buffer(& Dest, "DATA", 4);
    File_Write_Buffer(& Dest, (char *)(& Size), 8);
    File_Write_Buffer(& Dest, Data, Size);
    
    File_Close(& Dest);
    File_Dtor(& Dest);
    
    free(Data);
    
    return 1;
}
