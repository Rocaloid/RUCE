#include "Roto.h"
#include "IO.h"
#include "GetPara.h"
#include "WineUtil.h"
#include "Synth.h"
#include "Common.h"
#include <CVESVP.h>
#include <RUtil2.h>

#define CWineDir "/home/sleepwalking/.wine"

int main(int ArgN, char** Arg)
{
    RUCE_ResamplerPara Para;
    RUCE_ResamplerPara_Ctor(& Para);
    
    int i;
    for(i = 0; i < ArgN; i ++)
    {
        printf("%s\n", Arg[i]);
    }
    
    if(RUCE_ParsePara(& Para, ArgN, Arg) < 1)
        return 1;
    
    String Input, Output, WineDir;
    RNew(String, & Input, & Output, & WineDir);
    String_SetChars(& WineDir, CWineDir);
    
    WinePathToUnix(& Input, & Para.Input, & WineDir);
    WinePathToUnix(& Output, & Para.Output, & WineDir);
    
    printf("Input: %s, Output: %s\n",
        String_GetChars(& Input), String_GetChars(& Output));
    printf("InvarLeft: %f, InvarRight: %f\n", Para.InvarLeft, Para.InvarRight);
    printf("LenRequire: %f, FixedLength: %f\n", Para.LenRequire,
        Para.FixedLength);
    printf("Velocity: %f, Modulation: %f\n", Para.Velocity, Para.Modulation);
    printf("Volume: %f\n", Para.Volume);
    //PMatch_Float_Float_Print(& Para.Freq);
    
    String DirName, FileName, Dot, UnitName;
    RNew(String, & DirName, & FileName, & Dot, & UnitName);
    String_SetChars(& Dot, ".");
    DirFromFilePath(& DirName, & Input);
    BaseFromFilePath(& FileName, & Input);
    
    int DotPos = InStr(& FileName, & Dot);
    Left(& UnitName, & FileName, DotPos);
    
    String RotoPath, PMPath;
    RNew(String, & RotoPath, & PMPath);
    String_From(& RotoPath, & DirName);
    String_From(& PMPath, & DirName);
    String_JoinChars(& RotoPath, "/Roto.json");
    String_JoinChars(& PMPath, "/PitchModel.json");
    
    RUCE_DB_Entry DBEntry;
    RUCE_DB_Entry_Ctor(& DBEntry);
    int Ret = RUCE_DB_LoadEntry(& DBEntry, & UnitName, & DirName, & RotoPath);
    if(Ret < 1)
    {
        fprintf(stderr, "[Error] Cannot load unit '%s'.\n",
            String_GetChars(& UnitName));
        return 1;
    }
    
    _Wave InWave, OutWave;
    RNew(_Wave, & InWave, & OutWave);
    RCall(_Wave, Resize)(& InWave, DBEntry.WaveSize + 3000);
    RCall(_Wave, Write)(& InWave, DBEntry.Wave, 0, DBEntry.WaveSize);
    InWave.SampleRate = DBEntry.Samprate;
    OutWave.SampleRate = DBEntry.Samprate;
    
    CSVP_PitchModel PMEntry;
    CSVP_PitchModel_Ctor(& PMEntry);
    RUCE_LoadPitchModel(& PMEntry, & UnitName, & PMPath);
    
    SynthUnit(& OutWave, & InWave, & DBEntry, & PMEntry, & Para);
    
    printf("DirName: %s\n", String_GetChars(& DirName));
    printf("UnitName: %s\n", String_GetChars(& UnitName));
    
    if(! RCall(_Wave, ToFile)(& OutWave, & Output))
    {
        fprintf(stderr, "[Error] Cannot write to '%s'.\n",
            String_GetChars(& Output));
        return 1;
    }
    
    RDelete(& DBEntry, & InWave, & OutWave, & PMEntry);
    RDelete(& DirName, & FileName, & Dot, & UnitName, & RotoPath, & PMPath);
    RDelete(& Para, & Input, & Output, & WineDir);
    return 0;
}

