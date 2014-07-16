#include "RUCE.h"
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
    
    RDelete(& PM, & Path, & Name);
    return 0;
}

