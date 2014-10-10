#include "RUCEData.h"
#include <RUtil2.h>
#include <fnmatch.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include "../external/cJSON/cJSON.h"

static const int RUDB_Header = 0x42445552;
static const int _RUDB_Version_ = RUDB_VERSION;

RCtor(RUCE_DB_Frame)
{
    Array_Ctor(float, This -> Freq);
    Array_Ctor(float, This -> Ampl);
    Array_Ctor(float, This -> Phse);
    This -> Noiz = NULL;
    RInit(RUCE_DB_Frame);
}

RDtor(RUCE_DB_Frame)
{
    Array_Dtor(float, This -> Phse);
    Array_Dtor(float, This -> Ampl);
    Array_Dtor(float, This -> Freq);
    if(This -> Noiz)
        free(This -> Noiz);
}

RCtor(RUCE_DB_Entry)
{
    Array_Ctor(RUCE_DB_Frame, This -> FrameList);
    This -> ApWave = NULL;
    RInit(RUCE_DB_Entry);
}

RDtor(RUCE_DB_Entry)
{
    Array_ObjDtor(RUCE_DB_Frame, This -> FrameList);
    Array_Dtor(RUCE_DB_Frame, This -> FrameList);
    if(This -> ApWave)
        free(This -> ApWave);
}

#define _JSON_SafeReadList(Name) do {\
    cJSON* Name = cJSON_GetObjectItem(Entry, _S(Name)); \
    if(Name) \
    { \
        int ArrayNum = cJSON_GetArraySize(Name); \
        if(ArrayNum % 2 == 1) return -1; \
        PMatch_Float_Float_Clear(& Dest -> Name); \
        for(i = 0; i < ArrayNum / 2; i ++) \
        { \
            cJSON* Freq = cJSON_GetArrayItem(Name, i * 2); \
            cJSON* Val  = cJSON_GetArrayItem(Name, i * 2 + 1); \
            PMatch_Float_Float_AddPair(& Dest -> Name, \
                Freq -> valuedouble, Val -> valuedouble); \
        } \
    } \
    } while(0)

static int _RUCE_PitchModelFromJSON(CSVP_PitchModel* Dest, cJSON* Entry,
    cJSON* Top)
{
    int i;
    cJSON* Inherit = cJSON_GetObjectItem(Entry, "Inherit");
    if(Inherit && strcmp(Inherit -> valuestring, "none") != 0)
    {
        int EntryNum = cJSON_GetArraySize(Top);
        for(i = 0; i < EntryNum; i ++)
        {
            cJSON* Sub = cJSON_GetArrayItem(Top, i);
            char* Wildcard = cJSON_GetObjectItem(Sub, "Wildcard")
                          -> valuestring;
            if(! strcmp(Wildcard, Inherit -> valuestring))
            {
                _RUCE_PitchModelFromJSON(Dest, Sub, Top);
                break;
            }
        }
    }
    
    _JSON_SafeRead(LDecay_F0);
    _JSON_SafeRead(LDecay_Sensitivity);
    _JSON_SafeRead(LDecay_Center);
    _JSON_SafeRead(LDecay_Bound);
    _JSON_SafeRead(HDecay_F0);
    _JSON_SafeRead(HDecay_Sensitivity);
    _JSON_SafeRead(HDecay_Center);
    _JSON_SafeRead(HDecay_Bound);
    
    _JSON_SafeReadList(HmncCurve);
    _JSON_SafeReadList(NoizCurve);
    _JSON_SafeReadList(PhseCurve);
    
    return 1;
}

void RUCE_LoadPitchModel(CSVP_PitchModel* Dest, String* Sorc, String* Path)
{
    cJSON* Root, *Entries;
    File PMFile;
    String PMContent;
    String_Ctor(& PMContent);
    File_Ctor(& PMFile);
    
    if(! File_Open(& PMFile, Path, READONLY)) goto End;
    File_Read_String(& PMFile, & PMContent);
    
    Root = cJSON_Parse(String_GetChars(& PMContent));
    Entries = cJSON_GetObjectItem(Root, "Entries");
    
    int EntryNum = cJSON_GetArraySize(Entries);
    int i;
    cJSON* Match = NULL;
    for(i = 0; i < EntryNum; i ++)
    {
        cJSON* Entry = cJSON_GetArrayItem(Entries, i);
        char* Wildcard = cJSON_GetObjectItem(Entry, "Wildcard") -> valuestring;
        if(! fnmatch(Wildcard, String_GetChars(Sorc), 0))
        {
            //matched
            Match = Entry;
        }
    }
    
    if(Match)
    {
        _RUCE_PitchModelFromJSON(Dest, Match, Entries);
    }
    
    cJSON_Delete(Root);
    
    End:
    File_Close(& PMFile);
    File_Dtor(& PMFile);
    String_Dtor(& PMContent);
}

int RUCE_DB_LoadExternWave(RUCE_DB_Entry* Dest, String* Sorc, String* Path)
{
    WaveFile w;
    String l;
    String_Ctor(& l);
    
    WaveFile_Ctor(& w);
    
    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Sorc);
    String_JoinChars(& l, ".wav");
    
    /* Fetch WaveSize and wave */
    
    if(WaveFile_Open(& w, & l) != 1)
        return -1;
    if(w.Header.Channel != 1)
        return -1;
    Dest -> Samprate = w.Header.SamplePerSecond;
    Dest -> WaveSize = w.Header.DataNum;
    if(Dest -> ApWave) free(Dest -> ApWave);
    Dest -> ApWave = RAlloc_Float(w.Header.DataNum);
    WaveFile_FetchAllFloat(& w, Dest -> ApWave);
    
    WaveFile_Close(& w);
    
    WaveFile_Dtor(& w);
    String_Dtor(& l);
    return 1;
}

int RUCE_DB_RUDBWriteEntry(RUCE_DB_Entry* Sorc, String* Dest, String* Path)
{
    int Ret = -1;
    String l;
    String_Ctor(& l);
    
    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Dest);
    String_JoinChars(& l, ".rudb");
    if(RUCE_RUDB_Save(Sorc, & l) != 1)
        goto End;
    
    Ret = 1;
    
End:
    String_Dtor(& l);
    return Ret;
}

int RUCE_DB_WaveWriteEntry(RUCE_DB_Entry* Sorc, String* Dest, String* Path)
{
    int Ret = -1;
    String l;
    String_Ctor(& l);
    
    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Dest);
    String_JoinChars(& l, ".wav");
    
    WaveFile w;
    WaveFile_Ctor(& w);
    
    w.Header.Channel = 1;
    w.Header.SamplePerSecond = Sorc -> Samprate;
    w.Header.BytePerSecond = Sorc -> Samprate * 4;
    w.Header.BlockAlign = 1;
    w.Header.BitPerSample = 32;
    
    if(WaveFile_Save(& w, & l) != 1)
        goto End;
    
    WaveFile_WriteAllFloat(& w, Sorc -> ApWave, Sorc -> WaveSize);
    
    WaveFile_FinishWrite(& w);
    
    Ret = 1;
    
End:
    WaveFile_Dtor(& w);
    String_Dtor(& l);
    return Ret;
}

int RUCE_RUDB_Load(RUCE_DB_Entry* Dest, String* Path)
{
    File Sorc;
    File_Ctor(& Sorc);
    if(! File_Open(& Sorc, Path, READONLY))
        return -250;
    
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
        Endian_Switch_Array_UInt32(Header, 3);
        ReverseEndian = 1;
    }
    if(Header[0] != RUDB_Header)
    {
        //Bad Header
        return -2;
    }
    if(Header[2] > RUDB_VERSION)
        return -3;
    if(Header[2] == 1) // Remove it after severval months
    {
        //Old version
        return -32768;
    }
    if(Header[2] >= 3) // VOT, InvarLR are packed in RUDB
    {
        float VOT, SOT, InvarLeft, InvarRight;
        File_Read_Buffer(& Sorc, & VOT, 4);
        SOT = VOT;
        if(Header[2] >= 4)
            File_Read_Buffer(& Sorc, & SOT, 4);
        File_Read_Buffer(& Sorc, & InvarLeft, 4);
        File_Read_Buffer(& Sorc, & InvarRight, 4);
        if(ReverseEndian)
        {
            Endian_Switch_Float(& VOT);
            Endian_Switch_Float(& SOT);
            Endian_Switch_Float(& InvarLeft);
            Endian_Switch_Float(& InvarRight);
        }
        Dest -> VOT = VOT;
        Dest -> SOT = SOT;
        Dest -> InvarLeft = InvarLeft;
        Dest -> InvarRight = InvarRight;
    }
    File_Read_Buffer(& Sorc, CBuffer, 4);
    if(strncmp(CBuffer, "DATA", 4))
        return -4;
    File_Read_Buffer(& Sorc, & DataSize, 8);
    if(ReverseEndian) Endian_Switch_UInt64(& DataSize);
    if(DataSize < 8)
        return -5;
    
    char* Data = malloc(DataSize);
    File_Read_Buffer(& Sorc, Data, DataSize);
    
    UInt CorrectCRC = CRC32Sum(Data, DataSize, 0);
    if(Header[1] != CorrectCRC)
    {
        //Bad CRC32 checksum.
        return -6;
    }
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
        else if(! strncmp(Data, "WAVE", 4))
        {
            Data += 4;
            memcpy(& (Dest -> Samprate), Data, 4);
            Data += 4;
            memcpy(& (Dest -> WaveSize), Data, 4);
            Data += 4;
            if(ReverseEndian)
            {
                Endian_Switch_Int32(& (Dest -> Samprate));
                Endian_Switch_Int32(& (Dest -> WaveSize));
            }
            if(Dest -> ApWave) free(Dest -> ApWave);
            Dest -> ApWave = RAlloc_Float(Dest -> WaveSize);
            UInt SizeToCp = Dest -> WaveSize * sizeof(float);
            memcpy(Dest -> ApWave, Data, SizeToCp);
            Data += SizeToCp;
            
            if(ReverseEndian) 
                Endian_Switch_Array_Float(Dest -> ApWave, Dest -> WaveSize);
        }
        else if(! strncmp(Data, "EOL3", 4))
        {
            Data += 4;
            break;
        }
        else
        {
            //Bad block
            Data += 4;
            return -200;
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
    
    /*
      Calculate data size.
      HopSize, NoizSize, FRME, FrameCount, WAVE, Samprate, SampCnt, EOL3. (8*4)
      3 * 4 bytes fixed + 4 * NoizSize + 12 * Count per frame.
      8 + WaveSize * sizeof(float)
    */
    Size += 32 + FrameCnt * (12 + 4 * Sorc -> NoizSize) + 
                    Sorc -> WaveSize * sizeof(float);
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
    
    if(Sorc -> WaveSize)
    {
        memcpy(Curr, "WAVE", 4);
        Curr += 4;
        memcpy(Curr, & (Sorc -> Samprate), 4);
        Curr += 4;
        memcpy(Curr, & (Sorc -> WaveSize), 4);
        Curr += 4;
        UInt SizeToCp = Sorc -> WaveSize * sizeof(float);
        memcpy(Curr, Sorc -> ApWave, SizeToCp);
        Curr += SizeToCp;
    }
    
    memcpy(Curr, "EOL3", 4);
    
    int CRC = CRC32Sum(Data, Size, 0);
    File_Write_Buffer(& Dest, (char*)(& RUDB_Header), 4);
    File_Write_Buffer(& Dest, (char*)(& CRC), 4);
    File_Write_Buffer(& Dest, (char*)(& _RUDB_Version_), 4);
    File_Write_Buffer(& Dest, (char*)(& Sorc -> VOT), 4);
    File_Write_Buffer(& Dest, (char*)(& Sorc -> SOT), 4);
    File_Write_Buffer(& Dest, (char*)(& Sorc -> InvarLeft), 4);
    File_Write_Buffer(& Dest, (char*)(& Sorc -> InvarRight), 4);
    File_Write_Buffer(& Dest, "DATA", 4);
    File_Write_Buffer(& Dest, (char*)(& Size), 8);
    File_Write_Buffer(& Dest, Data, Size);
    
    File_Close(& Dest);
    File_Dtor(& Dest);
    
    free(Data);
    
    return 1;
}

void RUCE_DB_PrintEntry(RUCE_DB_Entry* Sorc)
{
    printf("RUCE_DB_Entry:\n"
           "    HopSize = %d, NoizSize = %d, Frame count = %d\n"
           "    FRME\n", 
           Sorc -> HopSize, Sorc -> NoizSize, Sorc -> FrameList_Index + 1);
    for(int i = 0; i <= Sorc -> FrameList_Index; ++ i)
    {
        printf("     |--Pos = %d, Cnt = %d\n", 
            Sorc -> FrameList[i].Position,
            Sorc -> FrameList[i].Freq_Index);
        for(int j = 0; j <= Sorc -> FrameList[i].Freq_Index; ++ j)
        {
            printf("     |  #%d: Freq = %f, Ampl = %f, Phse = %f.\n", j,
                Sorc -> FrameList[i].Freq[j], 
                Sorc -> FrameList[i].Ampl[j], 
                Sorc -> FrameList[i].Phse[j]);
        }
        printf("     |  Noiz = [");
        for(int j = 0; j < Sorc -> NoizSize; ++ j)
            printf("%f%s", Sorc -> FrameList[i].Noiz[j],
                j < Sorc -> NoizSize - 1 ? ", " : "");
        printf("]\n");
    }
    
    printf("    WAVE\n");
    printf("     |--WaveSize = %d, WaveSamprate = %d.\n", 
        Sorc -> WaveSize, Sorc -> Samprate);
    printf("     |  (WAVE DATA)\n");
    printf("    EOL3\n");
    
    printf("VOT = %f, InvarLeft = %f, InvarRight = %f.\n", 
        Sorc -> VOT, Sorc -> InvarLeft, Sorc -> InvarRight);
}

