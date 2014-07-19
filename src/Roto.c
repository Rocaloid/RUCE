#include "Roto.h"
#include "IO.h"
#include "../external/cJSON/cJSON.h"

RCtor(RUCE_Roto_Entry)
{
    String_Ctor(& This -> Name);
    This -> VOT = 0;
    This -> InvarLeft = 0;
    This -> InvarRight = 0;
    This -> Overlap = 0;
    RInit(RUCE_Roto_Entry);
}

RDtor(RUCE_Roto_Entry)
{
    String_Dtor(& This -> Name);
}

RCtor(RUCE_Roto)
{
    This -> Ptr = cJSON_CreateObject();
    cJSON* Entries = cJSON_CreateObject();
    cJSON_AddItemToObject(This -> Ptr, "Entries", Entries);
    
    RInit(RUCE_Roto);
}

RDtor(RUCE_Roto)
{
    cJSON_Delete(This -> Ptr);
}

int  RUCE_Roto_CtorLoad(RUCE_Roto* This, String* Path)
{
    int Ret = 1;
    
    File RotoFile;
    String RotoContent;
    String_Ctor(& RotoContent);
    File_Ctor(& RotoFile);
    
    if(! File_Open(& RotoFile, Path, READONLY))
    {
        Ret = 0;
        goto End;
    }
    File_Read_String(& RotoFile, & RotoContent);
    This -> Ptr = cJSON_Parse(String_GetChars(& RotoContent));
    
    End:
    File_Close(& RotoFile);
    File_Dtor(& RotoFile);
    String_Dtor(& RotoContent);
    
    RInit(RUCE_Roto);
    return Ret;
}

int  RUCE_Roto_Write(RUCE_Roto* This, String* Path)
{
    int Ret = 1;
    
    char* Data = cJSON_Print(This -> Ptr);
    File RotoFile;
    File_Ctor(& RotoFile);
    
    if(! File_Open(& RotoFile, Path, CREATE))
    {
        Ret = 0;
        goto End;
    }
    
    File_Write_Chars(& RotoFile, Data);
    File_Write_Chars(& RotoFile, "\n");
    
    End:
    File_Dtor(& RotoFile);
    free(Data);
    return Ret;
}

int  RUCE_Roto_GetEntry(RUCE_Roto* This, RUCE_Roto_Entry* Dest, String* Name)
{
    cJSON* Entries = cJSON_GetObjectItem(This -> Ptr, "Entries");
    if(! Entries) return 0;
    
    cJSON* Entry = cJSON_GetObjectItem(Entries, String_GetChars(Name));
    if(! Entry) return -1;
    
    String_Copy(& Dest -> Name, Name);
    
    _JSON_SafeRead(VOT);
    _JSON_SafeRead(InvarLeft);
    _JSON_SafeRead(InvarRight);
    _JSON_SafeRead(Overlap);
    
    return 1;
}

int  RUCE_Roto_SetEntry(RUCE_Roto* This, RUCE_Roto_Entry* Sorc)
{
    cJSON* Entries = cJSON_GetObjectItem(This -> Ptr, "Entries");
    if(! Entries) return 0;
    
    char* Name = String_GetChars(& Sorc -> Name);
    cJSON* Entry = cJSON_GetObjectItem(Entries, Name);
    
    cJSON* NewEntry = cJSON_CreateObject();
    cJSON_AddNumberToObject(NewEntry, "VOT", Sorc -> VOT);
    cJSON_AddNumberToObject(NewEntry, "InvarLeft", Sorc -> InvarLeft);
    cJSON_AddNumberToObject(NewEntry, "InvarRight", Sorc -> InvarRight);
    cJSON_AddNumberToObject(NewEntry, "Overlap", Sorc -> Overlap);
    if(! Entry)
    {
        //Create new entry
        cJSON_AddItemToObject(Entries, Name, NewEntry);
    }else
    {
        //Modify existing entry
        cJSON_ReplaceItemInObject(Entries, Name, NewEntry);
    }
    
    return 1;
}

