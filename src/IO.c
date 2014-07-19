#include "IO.h"
#include <RUtil2.h>
#include <fnmatch.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include "../external/cJSON/cJSON.h"

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
    Array_Ctor(int, This -> PulseList);
    This -> Wave = NULL;
    RInit(RUCE_DB_Entry);
}

RDtor(RUCE_DB_Entry)
{
    Array_ObjDtor(RUCE_DB_Frame, This -> FrameList);
    Array_Dtor(int, This -> PulseList);
    Array_Dtor(RUCE_DB_Frame, This -> FrameList);
    if(This -> Wave)
        free(This -> Wave);
}

#define _JSON_SafeRead(Name) do {\
    cJSON* Name = cJSON_GetObjectItem(Entry, _S(Name)); \
    if(Name) \
    { \
        Dest -> Name = Name -> valuedouble; \
    } \
    } while(0)

#define _JSON_SafeReadList(Name) do {\
    cJSON* Name = cJSON_GetObjectItem(Entry, _S(Name)); \
    if(Name) \
    { \
        int ArrayNum = cJSON_GetArraySize(Name); \
        if(ArrayNum % 2 == 1) return -1; \
        for(i = 0; i < ArrayNum / 2; i ++) \
        { \
            cJSON* Freq = cJSON_GetArrayItem(Name, i); \
            cJSON* Val  = cJSON_GetArrayItem(Name, i + 1); \
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

int RUCE_LoadRUDB(RUCE_DB_Entry* Dest, File* Sorc)
{
    int ReverseEndian = 0;
    
    int64_t FLen = File_GetLength(Sorc);
    if(FLen < 48)
        return -1;
    char CBuffer[8];
    uint32_t Header[4];
    uint64_t DataSize;
    File_Read_Buffer(Sorc, Header, 12);
    if(Header[0] != 0x42445552)
    {
        Endian_Switch_UInt32_Array(Header, 4);
        ReverseEndian = 1;
    }
    if(Header[0] != 0x42445552)
    {
        fprintf(stderr, "Bad file header.\n");
        return -1;
    }
    File_Read_Buffer(Sorc, CBuffer, 4);
    if(strncmp(CBuffer, "DATA", 4))
    {
        fprintf(stderr, "Bad DATA header.\n");
        return -2;
    }
    
    File_Read_Buffer(Sorc, & DataSize, 8);
    
    if(DataSize < 8)
    {
        fprintf(stderr, "Bad Data size.\n");
        return -2;
    }
    
    char* Data = malloc(DataSize);
    File_Read_Buffer(Sorc, Data, DataSize);
    
    if(Header[1] != CRC32Sum(Data, DataSize, 0))
    {
        fprintf(stderr, "Bad CRC32 sum.\n");
        return -2;
    }
    
    memcpy(& (Dest -> HopSize), Data, 8);
    Data += 8;
    if(ReverseEndian)
        Endian_Switch_Int32_Array(& (Dest -> HopSize), 2);
    
    while(Data != Data + DataSize)
    {
        if(! strncmp(Data, "FRME", 4))
        {
            Data += 4;
            memcpy(Header, Data, 4);
            Data += 4;
            if(ReverseEndian)
                Endian_Switch_UInt32(Header);
            Array_Resize(RUCE_DB_Frame, Dest -> FrameList, Header[0]);
            for(int i = 0; i < Header[0]; ++i)
            {
                RUCE_DB_Frame_Ctor(& (Dest -> FrameList[i]));
                if(strncmp(Data, "FRMB", 4))
                {
                    fprintf(stderr, 
                            "[ERROR] Bad FRMB @ %p!\n", 
                            Data);
                    return -255;
                }
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
                    Endian_Switch_Float_Array(Dest -> FrameList[i].Freq, Cnt);
                    Endian_Switch_Float_Array(Dest -> FrameList[i].Ampl, Cnt);
                    Endian_Switch_Float_Array(Dest -> FrameList[i].Phse, Cnt);
                    Endian_Switch_Float_Array(Dest -> FrameList[i].Noiz, Dest -> NoizSize);
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
            Array_Resize(int, Dest -> PulseList, Header[0]);
            memcpy(Dest -> PulseList, Data, Header[0] * 4);
            Data += Header[0] * 4;
            if(ReverseEndian)
                Endian_Switch_Int32_Array(Dest -> PulseList, Header[0]);
        }
        else if(! strncmp(Data, "EOL3", 4))
        {
            Data += 4;
            break;
        }
        else
        {
            Data += 4;
            fprintf(stderr, 
                    "[ERROR] Bad RUDB @ %p!\n", 
                    Data);
            return -233;
        }
    }
    
    free(Data - DataSize);
    return 1;
}

int RUCE_DB_ReadEntry(RUCE_DB_Entry* Dest, String* Sorc, String* Path, String* RotoPath)
{
    int Ret = -1;
    cJSON* Root, * Entries;
    String s, l;
    File f;
    
    String_Ctor(& s);
    String_Ctor(& l);
    File_Ctor(& f);
    
    /* Load and phrase Roto */
    
    if(! File_Open(& f, RotoPath, READONLY)) goto End;
    File_Read_String(& f, & s);
    
    Root = cJSON_Parse(String_GetChars(& s));
    Entries = cJSON_GetObjectItem(Root, "Entries");
    
    cJSON* Entry = cJSON_GetObjectItem(Entries, String_GetChars(Sorc));
    
    _JSON_SafeRead(VOT);
    _JSON_SafeRead(InvarLeft);
    _JSON_SafeRead(InvarRight);
    
    cJSON_Delete(Root);
    
    File_Close(& f);
    
    /* Load RUDB. */

    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Sorc);
    String_JoinChars(& l, ".rudb");
    if(! File_Open(& f, & l, READONLY)) goto End;
    if(RUCE_LoadRUDB(Dest, & f) != 1)
    {
        fprintf(stderr, "[Error] There are some errors occurred while reading RUDB!\n");
        goto End;
    }
    
    File_Close(& f);
    
    WaveFile w;
    WaveFile_Ctor(& w);
    
    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Sorc);
    String_JoinChars(& l, ".wav");
    
    /* Fetch Wavesize and wave */
    
    if(WaveFile_Open(& w, & l) != 1)
    {
        fprintf(stderr, "[Error] Cannot open wave file '%s'!\n", String_GetChars(& l));
        goto End;
    }
    if(w.Header.Channel != 1)
    {
        fprintf(stderr, "[Error] We only support mono wav yet!\n");
        goto End;
    }
    Dest -> WaveSize = w.Header.DataNum;
    if(Dest -> Wave)
        free(Dest -> Wave);
    Dest -> Wave = RAlloc_Float(w.Header.DataNum);
    WaveFile_FetchAllFloat(& w, Dest -> Wave);
    
    WaveFile_Close(& w);
    
    WaveFile_Dtor(& w);
    
    Ret = 1;
    
End:
    File_Close(& f);
    File_Dtor(& f);
    String_Dtor(& s);
    String_Dtor(& l);
    return Ret;
}

int RUCE_DB_WriteEntry(RUCE_DB_Entry* Sorc, String* Dest, String* Path, String* RotoPath)
{
    int Ret = -1;
    cJSON* Root, * Entries;
    String s, l;
    File f;
    
    
    
    String_Ctor(& s);
    String_Ctor(& l);
    File_Ctor(& f);
    
End:
    File_Close(& f);
    File_Dtor(& f);
    String_Dtor(& s);
    String_Dtor(& l);
    return Ret;
}

void RUCE_DB_PrintEntry(RUCE_DB_Entry* Sorc)
{
    printf("RUCE_DB_Entry:\n"
           "    HopSize = %d, NoizSize = %d, \n"
           "    Frame count = %d, Pulse count = %d, \n"
           "    FRME\n", 
           Sorc -> HopSize, Sorc -> NoizSize, 
           Sorc -> FrameList_Index + 1, Sorc -> PulseList_Index + 1);
    for(int i = 0; i <= Sorc -> FrameList_Index; ++ i)
    {
        printf("     |--Pos = %d, Cnt = %d\n", 
               Sorc -> FrameList[i].Position,
               Sorc -> FrameList[i].Freq_Index);
        for(int j = 0; j <= Sorc -> FrameList[i].Freq_Index; ++ j)
        {
            printf("     |  #%d: Freq = %f, Ampl = %f, Phse = %f.\n", 
                   j, Sorc -> FrameList[i].Freq[j], 
                      Sorc -> FrameList[i].Ampl[j], 
                      Sorc -> FrameList[i].Phse[j]);
        }
        printf("     |  Noiz = [");
        for(int j = 0; j < Sorc -> NoizSize; ++ j)
            printf("%f, ", Sorc -> FrameList[i].Noiz[j]);
        printf("\b\b]\n");
    }
    
    printf("    PULS\n");
    for(int i = 0; i <= Sorc -> PulseList_Index; ++ i)
        printf("     |--#%d = %d\n", i, Sorc -> PulseList[i]);
    printf("     |__EOL3\n");
    
    printf("    WaveSize = %d, VOT = %d, InvarLeft = %d, InvarRight = %d.\n", 
           Sorc -> WaveSize, Sorc -> VOT, Sorc -> InvarLeft, Sorc -> InvarRight);
}

int main()
{
    String_FromChars(a, "a");
    String_FromChars(b, "./");
    String_FromChars(c, "./roto.json");
    RUCE_DB_Entry t;
    RUCE_DB_Entry_Ctor(& t);
    printf("Done, Ret = %d.\n", RUCE_DB_GetEntry(& t, & a, & b, & c));
    RUCE_DB_PrintEntry(& t);
    String_Dtor(& c);
    String_Dtor(& b);
    String_Dtor(& a);
    RUCE_DB_Entry_Dtor(& t);
    return 0;
}