#include "Roto.h"
#include "IO.h"
#include <CVESVP.h>

int main(int ArgN, char** ArgList)
{
    CSVP_PitchModel PM;
    CSVP_PitchModel_Ctor(& PM);
    
    String Path, Name;
    RNew(String, & Path, & Name);
    String_SetChars(& Path, "/tmp/testpm.json");
    String_SetChars(& Name, "bie");
    
    RUCE_LoadPitchModel(& PM, & Name, & Path);
    printf("%f\n", PMatch_Float_Float_Query(& PM.NoizCurve, 300).Y);
    
    RUCE_Roto Roto;
//    RUCE_Roto_Ctor(& Roto);
    RUCE_Roto_Entry Entry;
    RUCE_Roto_Entry_Ctor(& Entry);
    
    String_SetChars(& Path, "/tmp/testroto.json");
    RUCE_Roto_CtorLoad(& Roto, & Path);
    RUCE_Roto_GetEntry(& Roto, & Entry, & Name);
    String_SetChars(& Entry.Name, "bao");
    
    Entry.VOT = 1234;
    RUCE_Roto_SetEntry(& Roto, & Entry);
    
    String_SetChars(& Path, "/tmp/testroto2.json");
    RUCE_Roto_Write(& Roto, & Path);
    
    RDelete(& PM, & Path, & Name, & Roto, & Entry);
    return 0;
}

