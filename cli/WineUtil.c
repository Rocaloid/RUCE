#include "WineUtil.h"
#include <ctype.h>

void WinePathToUnix(String* Dest, String* Sorc, String* WineDir)
{
    int i;
    String TempStr;
    String_Ctor(& TempStr);
    
    if(Sorc -> Data[1] != ':')
    {
        String_From(Dest, Sorc);
        return;
    }

    for(i = 0; i < String_GetLength(Sorc); i ++)
        if(Sorc -> Data[i] == '\\')
            Sorc -> Data[i] = '/';
    if(Sorc -> Data[0] == 'Z')
    {
        MidFrom(Dest, Sorc, 2);
    }else
    {
        char DriveNo[2];
        DriveNo[0] = tolower(Sorc -> Data[0]);
        DriveNo[1] = '\0';
        String_From(Dest, WineDir);
        String_JoinChars(Dest, "/drive_");
        String_JoinChars(Dest, (char*)DriveNo);
        MidFrom(& TempStr, Sorc, 2);
        String_Join(Dest, & TempStr);
    }
    
    String_Dtor(& TempStr);    
}

