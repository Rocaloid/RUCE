#include "IO.h"
#include <RUtil2.h>
#include "../external/cJSON/cJSON.h"

void RUCE_LoadPitchModel(CSVP_PitchModel* Dest, String* Sorc, String* Path)
{
    cJSON* Root;
    File PMFile;
    String PMContent;
    String_Ctor(& PMContent);
    File_Ctor(& PMFile);
    
    if(! File_Open(& PMFile, Path, READONLY)) goto End;
    File_Read_String(& PMFile, & PMContent);
    Root = cJSON_Parse(String_GetChars(& PMContent));
    
    cJSON_Delete(Root);
    
    End:
    File_Close(& PMFile);
    File_Dtor(& PMFile);
    String_Dtor(& PMContent);
}

