#include "IO.h"
#include <RUtil2.h>
#include <fnmatch.h>
#include <stdint.h>
#include <string.h>
#include <memory.h>
#include "Roto.h"
#include "RUDB.h"
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

int 
RUCE_DB_LoadEntry
(RUCE_DB_Entry* Dest, String* Sorc, String* Path, String* RotoPath)
{
    int Ret = -1;
    String s, l;
    
    String_Ctor(& s);
    String_Ctor(& l);
    
    /* Load Roto */
    
    RUCE_Roto       o;
    RUCE_Roto_Entry e;
    if(RUCE_Roto_CtorLoad(& o, RotoPath) != 1)
        goto End;
    RUCE_Roto_Entry_Ctor(& e);
    
    RUCE_Roto_GetEntry(& o, & e, Sorc);
    Dest -> VOT = e.VOT;
    Dest -> InvarLeft = e.InvarLeft;
    Dest -> InvarRight = e.InvarRight;
    
    RUCE_Roto_Entry_Dtor(& e);
    RUCE_Roto_Dtor(& o);
    
    /* Load RUDB. */

    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Sorc);
    String_JoinChars(& l, ".rudb");
    
    if(RUCE_RUDB_Load(Dest, & l) != 1)
        goto End;
    
    WaveFile w;
    WaveFile_Ctor(& w);
    
    String_Copy(& l, Path);
    String_JoinChars(& l, "/");
    String_Join(& l, Sorc);
    String_JoinChars(& l, ".wav");
    
    /* Fetch Wavesize and wave */
    
    if(WaveFile_Open(& w, & l) != 1)
        goto End;
    if(w.Header.Channel != 1)
        goto End;
    Dest -> WaveSize = w.Header.DataNum;
    if(Dest -> Wave)
        free(Dest -> Wave);
    Dest -> Wave = RAlloc_Float(w.Header.DataNum);
    WaveFile_FetchAllFloat(& w, Dest -> Wave);
    
    WaveFile_Close(& w);
    
    WaveFile_Dtor(& w);
    
    Ret = 1;
    
End:
    String_Dtor(& s);
    String_Dtor(& l);
    return Ret;
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

int RUCE_DB_RotoWriteEntry(RUCE_DB_Entry* Sorc, String* Name, String* RotoPath)
{
    int Ret = -1;
    
    RUCE_Roto       o;
    RUCE_Roto_Entry e;
    
    if(RUCE_Roto_CtorLoad(& o, RotoPath) != 1)
        RUCE_Roto_Ctor(& o);
    
    RUCE_Roto_Entry_Ctor(& e);
    
    String_Copy(& e.Name, Name);
    e.VOT = Sorc -> VOT;
    e.InvarLeft = Sorc -> InvarLeft;
    e.InvarRight = Sorc -> InvarRight;
    
    RUCE_Roto_SetEntry(& o, & e);
    
    if(RUCE_Roto_Write(& o, RotoPath) != 1)
        goto End;
    
    Ret = 1;
    
End:
    RUCE_Roto_Entry_Dtor(& e);
    RUCE_Roto_Dtor(& o);
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
    w.Header.SamplePerSecond = 44100;
    w.Header.BytePerSecond = 176400;
    w.Header.BlockAlign = 1;
    w.Header.BitPerSample = 32;
    
    if(WaveFile_Save(& w, & l) != 1)
        goto End;
    
    WaveFile_WriteAllFloat(& w, Sorc -> Wave, Sorc -> WaveSize);
    
    WaveFile_FinishWrite(& w);
    
    Ret = 1;
    
End:
    WaveFile_Dtor(& w);
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
        printf("     |  #%d = %d\n", i, Sorc -> PulseList[i]);
    printf("    EOL3\n");
    
    printf("    WaveSize = %d," 
           "VOT = %d, InvarLeft = %d, InvarRight = %d.\n", 
           Sorc -> WaveSize, 
           Sorc -> VOT, Sorc -> InvarLeft, Sorc -> InvarRight);
}
