#include "RUCEData.h"
#include "Soundbank.h"

void* RUCE_CreateLoadSoundbank(char* Directory)
{
    RUCE_Soundbank* Ret = (RUCE_Soundbank*)malloc(sizeof(RUCE_Soundbank));
    String_Ctor(& Ret -> Directory);
    String_SetChars(& Ret -> Directory, Directory);
    CSVP_PitchModel_Ctor(& Ret -> PMDefault);
    Ret -> PMRoot = NULL;
    Ret -> PMEntries = NULL;
    
    File PMFile;
    String PMContent;
    String_Ctor(& PMContent);
    File_Ctor(& PMFile);
    
    if(! File_OpenChars(& PMFile, Directory, READONLY)) goto End;
    File_Read_String(& PMFile, & PMContent);
    
    Ret -> PMRoot = cJSON_Parse(String_GetChars(& PMContent));
    if(! Ret -> PMRoot) goto End;
    Ret -> PMEntries = cJSON_GetObjectItem(Ret -> PMRoot, "Entries");
    if(! Ret -> PMEntries) goto End;
    
    End:
    File_Dtor(& PMFile);
    String_Dtor(& PMContent);
    return (void*)Ret;
}

int RUCE_DestroySoundbank(RUCE_Soundbank* Soundbank)
{
    String_Dtor(& Soundbank -> Directory);
    CSVP_PitchModel_Dtor(& Soundbank -> PMDefault);
    free(Soundbank);
    return 1;
}

