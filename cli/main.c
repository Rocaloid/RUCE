#include "libRUCE.h"
#include <RUtil2.h>

int main()
{
    const RUCE_Version* Ver = RUCE_GetVersion();
    printf("libRUCE version: %d.%d.%d(%d)-%s\n",
        Ver -> Major, Ver -> Minor, Ver -> Revision, Ver -> Patch, Ver -> Tag);
    printf("libRUCE API version: %d.%d.%d\n",
        Ver -> APIMajor, Ver -> APIMinor, Ver -> APIRevision);
    
    int i;
    double Notes_Time[] = {1  , 1.5, 2, 3, 3.5};
    double Notes_Dur[]  = {0.4, 0.5, 1, 0.5, 1};
    char*  Notes_Lyric[] = {"a", "la", "la", "ka", "kuan"};
    
    RUCE_Session* Main = RUCE_CreateSynthSession(44100, 44100 * 30);
    RUCE_Soundbank* Bank = RUCE_CreateLoadSoundbank(
        "/tmp/SoundbankBuilder/home/Cyan-RUCE-Source-masters");
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
    
    printf("%d\n", RUCE_SessionSynthStep(Main, Output, 0.5));
    printf("%d\n", RUCE_SessionSynthStep(Main, Output, 1.3));
    printf("%d\n", RUCE_SessionSynthStep(Main, Output, 2.0));
    printf("%d\n", RUCE_SessionSynthStep(Main, Output, 2.9));
    printf("%d\n", RUCE_SessionSynthStep(Main, Output, 3.5));
    printf("%d\n", RUCE_SessionSynthStep(Main, Output, 4.0));
    
    RFree(Output);
    RUCE_DestroySynthSession(Main);
    RUCE_DestroySoundbank(Bank);
    
    return 1;
}

