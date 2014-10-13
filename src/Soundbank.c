#include "RUCEData.h"
#include "Soundbank.h"

RUCE_Soundbank* RUCE_CreateLoadSoundbank(char* Directory)
{
    _RUCE_Soundbank* Ret = (_RUCE_Soundbank*)malloc(sizeof(_RUCE_Soundbank));
    String_Ctor(& Ret -> Directory);
    String_SetChars(& Ret -> Directory, Directory);
    CSVP_PitchModel_Ctor(& Ret -> PMDefault);
    Ret -> PMRoot = NULL;
    Ret -> PMEntries = NULL;
    
    File PMFile;
    String PMContent;
    String_Ctor(& PMContent);
    File_Ctor(& PMFile);
    
    char* PMPath = malloc(strlen(Directory) + 30);
    strcpy(PMPath, Directory);
    strcat(PMPath, "/PitchModel.json");
    printf("%s\n", PMPath);
    if(! File_OpenChars(& PMFile, PMPath, READONLY)) goto End;
    File_Read_String(& PMFile, & PMContent);
    
    Ret -> PMRoot = cJSON_Parse(String_GetChars(& PMContent));
    if(! Ret -> PMRoot) goto End;
    Ret -> PMEntries = cJSON_GetObjectItem(Ret -> PMRoot, "Entries");
    if(! Ret -> PMEntries) goto End;
    
    End:
    free(PMPath);
    File_Dtor(& PMFile);
    String_Dtor(& PMContent);
    return (RUCE_Soundbank*)Ret;
}

int RUCE_DestroySoundbank(RUCE_Soundbank* Soundbank)
{
    _RUCE_Soundbank* Bank = Soundbank;
    if(Bank -> PMRoot)
        cJSON_Delete(Bank -> PMRoot);
    String_Dtor(& Bank -> Directory);
    CSVP_PitchModel_Dtor(& Bank -> PMDefault);
    free(Soundbank);
    return 1;
}

int RUCE_SoundbankLoadEntry(RUCE_DB_Entry* Dest, RUCE_Soundbank* Bank,
    String* Name)
{
    String RudbPath;
    String_Ctor(& RudbPath);
    _RUCE_Soundbank* _Bank = Bank;
    
    String_From(& RudbPath, & _Bank -> Directory);
    String_JoinChars(& RudbPath, "/");
    String_Join(& RudbPath, Name);
    String_JoinChars(& RudbPath, ".rudb");
    
    int Ret = RUCE_RUDB_Load(Dest, & RudbPath);
    printf("Load %s, Ret=%d\n", String_GetChars(& RudbPath), Ret);
    
    String_Dtor(& RudbPath);
    return Ret;
}

int RUCE_SoundbankLoadPitchModel(CSVP_PitchModel* Dest, RUCE_Soundbank* Bank,
    String* Name)
{
    _RUCE_Soundbank* _Bank = Bank;
    if(_Bank -> PMEntries)
        return RUCE_PitchModelFromJSONEntries(Dest, _Bank -> PMEntries, Name);
    else
        CSVP_PitchModel_From(Dest, & _Bank -> PMDefault);
    return 0;
}

