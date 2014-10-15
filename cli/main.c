#include "libRUCE.h"
#include <CVEDSP2.h>
#include <RUtil2.h>
#include "Common.h"

int main()
{
    const RUCE_Version* Ver = RUCE_GetVersion();
    printf("libRUCE version: %d.%d.%d(%d)-%s\n",
        Ver -> Major, Ver -> Minor, Ver -> Revision, Ver -> Patch, Ver -> Tag);
    printf("libRUCE API version: %d.%d.%d\n",
        Ver -> APIMajor, Ver -> APIMinor, Ver -> APIRevision);
    
    RUCE_SetVerboseLevel(5);
    
    int i;
    double Notes_Time[] = {1  , 1.5, 2, 3, 3.5};
    double Notes_Dur[]  = {0.4, 0.5, 1, 0.5, 1};
    double Notes_DurC[] = {0.05, 0.05, 0.1, 0.1, 0.05};
    char*  Notes_Lyric[] = {"a", "la", "la", "ka", "kuan"};
    
    RUCE_Session* Main = RUCE_CreateSynthSession(44100, 44100 * 30);
    RUCE_Soundbank* Bank = RUCE_CreateLoadSoundbank(
        "/tmp/SoundbankBuilder/home/Cyan-RUCE-Source-master");
    RUCE_SessionSetSoundbank(Main, Bank);
    
    if(! Main)
    {
        fprintf(stderr, "[Error] Cannot create synth session!\n");
        return 0;
    }
    for(i = 0; i < 5; i ++)
    {
        RUCE_Note* Note = RUCE_CreateNote();
        Note -> Duration = Notes_Dur[i];
        Note -> CParamSet.DurConsonant = Notes_DurC[i];
        strcpy(Note -> Lyric, Notes_Lyric[i]);
        if(RUCE_SessionAppendNote(Main, Note, Notes_Time[i]) != 1)
            fprintf(stderr, "[Error] Cannot append note at %f\n",
                Notes_Time[i]);
        RUCE_DestroyNote(Note);
    }
    
    RUCE_SessionAppendEvent(Main, RUCE_EFREQ, 0, 262.0);
    RUCE_SessionAppendEvent(Main, RUCE_EFREQ, 1, 262.0);
    RUCE_SessionAppendEvent(Main, RUCE_EFREQ, 1.5, 270.0);
    RUCE_SessionAppendEvent(Main, RUCE_EFREQ, 1.9, 270.0);
    RUCE_SessionAppendEvent(Main, RUCE_EFREQ, 2.0, 320.0);
    RUCE_SessionAppendEvent(Main, RUCE_EFREQ, 3.5, 320.0);
    RUCE_SessionAppendEvent(Main, RUCE_EAMPL, 0, 1.0);
    RUCE_SessionAppendEvent(Main, RUCE_EBRE, 0, 0.5);
    RUCE_SessionAppendEvent(Main, RUCE_EGEN, 0, 0.5);
    
    RUCE_SessionSynthInit(Main, 0);
    
    float* Output = RAlloc_Float(44100 * 10);
    
    Wave OutWave;
    RCall(Wave, CtorSize)(& OutWave, 44100 * 10);
    double Head = 0;
    int OutHead = 0;
    while(Head < 4)
    {
        int Ret = RUCE_SessionSynthStep(Main, Output, Head + 0.5);
        printf("%d\n", Ret);
        if(Ret == -2)
            Ret = RUCE_SessionSynthStep(Main, Output, Head + 0.5);
        for(i = 0; i < Ret; i ++)
            OutWave.Data[OutHead + i] = Output[i];
        OutHead += Ret;
        Head += 0.2;
    }
    String_FromChars(OutPath, "/tmp/out.wav");
    RCall(Wave, ToFile)(& OutWave, & OutPath);
    
    RDelete(& OutWave, & OutPath);
    
    RFree(Output);
    RUCE_DestroySynthSession(Main);
    RUCE_DestroySoundbank(Bank);
    
    return 1;
}

