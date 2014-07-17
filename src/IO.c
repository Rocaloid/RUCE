#include "IO.h"
#include <RUtil2.h>
#include <fnmatch.h>
#include "../external/cJSON/cJSON.h"

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


