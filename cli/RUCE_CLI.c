#include "Roto.h"
#include "IO.h"
#include "GetPara.h"
#include "WineUtil.h"
#include "Synth.h"
#include "Common.h"
#include <CVESVP.h>
#include <RUtil2.h>

#define CWineDir "/home/sleepwalking/.wine"

#ifdef _WIN64
#define _WIN
#endif
#ifdef _WIN32
#define _WIN
#endif

int main(int ArgN, char** Arg)
{
    RUCE_ResamplerPara Para;
    RUCE_ResamplerPara_Ctor(& Para);
    printf("RUCE - Rocaloid UTAU Compatible Engine (1.0.0-alpha2)\n");
    printf("  License: GNU GPL v3 (http://www.gnu.org/licenses/gpl-3.0.txt)\n");
    printf("  www.rocaloid.org\n");
    
    int i;
    
    if(RUCE_ParsePara(& Para, ArgN, Arg) < 1)
    {
        fprintf(stderr, "Argument parsing failed. Argument log:\n");
        for(i = 0; i < ArgN; i ++)
            fprintf(stderr, "  #%d: %s\n", i, Arg[i]);
        return 1;
    }
    
    printf("Argument parsing done.\n");
    
    String Input, Output, WineDir;
    RNew(String, & Input, & Output, & WineDir);
    String_SetChars(& WineDir, CWineDir);
    
    if(Para.FlagPara.Verbose)
    {
        printf("Arguments:\n");
        for(i = 0; i < ArgN; i ++)
            printf("  #%d: %s\n", i, Arg[i]);
    }
    
    #ifdef _WIN
    String_From(& Input, & Para.Input);
    String_From(& Output, & Para.Output);
    #else
    WinePathToUnix(& Input, & Para.Input, & WineDir);
    WinePathToUnix(& Output, & Para.Output, & WineDir);
    #endif
    
    if(Para.FlagPara.Verbose)
    {
        printf("Input: %s, Output: %s\n",
            String_GetChars(& Input), String_GetChars(& Output));
        printf("InvarLeft: %f, InvarRight: %f\n", Para.InvarLeft,
            Para.InvarRight);
        if(Para.LenRequire < 0)
            printf("LenRequire: (keep), FixedLength: %f\n", Para.FixedLength);
        else
            printf("LenRequire: %f, FixedLength: %f\n", Para.LenRequire,
                Para.FixedLength);
        printf("Velocity: %f, Modulation: %f\n", Para.Velocity,
            Para.Modulation);
        printf("Volume: %f\n", Para.Volume);
    }
    
    String DirName, FileName, Dot, UnitName;
    RNew(String, & DirName, & FileName, & Dot, & UnitName);
    String_SetChars(& Dot, ".");
    DirFromFilePath(& DirName, & Input);
    BaseFromFilePath(& FileName, & Input);
    
    int DotPos = InStr(& FileName, & Dot);
    Left(& UnitName, & FileName, DotPos);
    
    String RotoPath, OtoPath, PMPath;
    RNew(String, & RotoPath, & OtoPath, & PMPath);
    String_From(& RotoPath, & DirName);
    String_From(& OtoPath, & DirName);
    String_From(& PMPath, & DirName);
    String_JoinChars(& RotoPath, "/Roto.json");
    String_JoinChars(& OtoPath, "/oto.ini");
    String_JoinChars(& PMPath, "/PitchModel.json");
    
    printf("DirName: %s\n", String_GetChars(& DirName));
    printf("UnitName: %s\n", String_GetChars(& UnitName));
    
    RUCE_DB_Entry DBEntry;
    RUCE_DB_Entry_Ctor(& DBEntry);
    RUCE_Oto_Entry OtoEntry;
    RUCE_Oto_Entry_Ctor(& OtoEntry);
    int Ret = RUCE_DB_LoadEntry(& DBEntry, & UnitName, & DirName, & RotoPath);
    
    if(Ret < 1)
    {
        fprintf(stderr, "[Error] Cannot load unit '%s'.\n",
            String_GetChars(& UnitName));
        return 1;
    }
    
    Ret = RUCE_Oto_LoadEntry(& OtoEntry, & UnitName, & OtoPath);
    if(Ret < 1)
    {
        fprintf(stderr, "[Error] Cannot find unit '%s' from oto.ini.\n",
            String_GetChars(& UnitName));
        return 1;
    }
    
    _Wave InWave, OutWave;
    RNew(_Wave, & InWave, & OutWave);
    RCall(_Wave, Resize)(& InWave, DBEntry.WaveSize + 3000);
    RCall(_Wave, Write)(& InWave, DBEntry.Wave, 0, DBEntry.WaveSize);
    InWave.SampleRate = DBEntry.Samprate;
    OutWave.SampleRate = DBEntry.Samprate;
    if(Para.LenRequire < 0)
        Para.LenRequire = (Real)TopOf(DBEntry.FrameList).Position
                        / InWave.SampleRate;
    Para.LenRequire += Para.FlagPara.DeltaDuration;
    if(Para.FlagPara.DeltaDuration != 0)
        printf("Adjusted duartion = %f sec.\n", Para.LenRequire);
    
    CSVP_PitchModel PMEntry;
    CSVP_PitchModel_Ctor(& PMEntry);
    RUCE_LoadPitchModel(& PMEntry, & UnitName, & PMPath);
    
    RUCE_SynthUnit(& OutWave, & InWave, & DBEntry, & PMEntry, & Para);
    
    printf("Synthesis successfully finished.\n");
    
    if(! RCall(_Wave, ToFile)(& OutWave, & Output))
    {
        fprintf(stderr, "[Error] Cannot write to '%s'.\n",
            String_GetChars(& Output));
        return 1;
    }
    
    RDelete(& DBEntry, & InWave, & OutWave, & PMEntry, & OtoEntry);
    RDelete(& DirName, & FileName, & Dot, & UnitName, & RotoPath, & PMPath,
        & OtoPath);
    RDelete(& Para, & Input, & Output, & WineDir);
    return 0;
}

