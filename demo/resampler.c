/*
  Obsoleted.
*/


#define  __CSVP_Install

#include <CVESVP/PSOLA.h>
#include <CVESVP/EndPoint.h>
#include <CVEDSP2.h>
#include <RUtil2.h>

#define number Float
#define CNumberStr CFloatStr

#undef  PMatch

#define Wave _C(CDSP2_Wave, _, number)
#define InfWave _C(CDSP2_InfWave, _, number)
#define IWave _C(CDSP2_IWave, _, number)
#define PSOLAIterlyzer _C(CSVP_PSOLAIterlyzer, _, number)
#define PSOLAItersizer _C(CSVP_PSOLAItersizer, _, number)
#define List_DataFrame _C(CSVP_List_DataFrame, _, number)
#define List_Int CSVP_List_Int
#define FWindow_T _C(RFNL_FWindow_Gnrc, _, number)
#define FWindow _C(CDSP2_FWindow, _, number)
#define Transition _C(Transition, _, number, _, number)
#define PMatch _C(PMatch, _, number, _, number)

char* PitchList[] =
{
    "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
    "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
    "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
    "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5"
};

number FreqList[] =
{
    65.40639, 69.29565, 73.41619, 77.78175, 82.40689, 87.30706,
    92.49860, 97.99886, 103.8262, 110.0000, 116.5409, 123.4708,
    129.9053, 137.7331, 145.8967, 154.5894, 163.8225, 173.5986,
    183.9081, 194.7262, 206.4516, 218.6788, 231.8841, 245.5243,
    260.1626, 275.8621, 291.7933, 308.6817, 327.6451, 346.5704,
    366.4122, 388.6640, 412.0172, 436.3636, 463.7681, 489.7959,
    518.9189, 551.7241, 581.8182, 619.3549, 653.0612, 695.6522,
    732.8244, 780.4878, 827.5862, 872.7273, 923.0769, 979.5919,
    1043.478, 1108.730, 1174.659, 1244.508, 1318.510, 1396.913,
    1479.978, 1567.982, 1661.219, 1760.000, 1864.655, 1975.533
};

number FreqFromPitch(String* Sorc)
{
    int i;
    for(i = 0; i < 60; i ++)
        if(String_EqualChars(Sorc, PitchList[i]))
            return FreqList[i] / 2;
    return 0;
}

void PMatchFromList_Int(PMatch* Dest, List_Int* Sorc)
{
    int i;
    for(i = Sorc -> Frames_Index; i >= 0; i --)
        RCall(PMatch, AddPair)(Dest, Sorc -> Frames[i], i);
}

//resampler <wav in> <wav out> <pitch> <velocity> <flags>
int main(int ArgQ, char** ArgList)
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    CDSP2_SetDebugOn(CDSP2_Debug_Check);
    
    printf("Sleepwalking's toy resampler v0.1.\n");
    printf("Powered by Rocaloid's subprojects: CVESVP, CVEDSP2, and RFNL.\n");
    
    String OutputPath, InputPath, StrPitch, StrVelocity, StrFlags, StrOffset,
        StrLength;
    RNew(String, & OutputPath, & InputPath, & StrPitch, & StrVelocity, 
        & StrFlags, & StrOffset, & StrLength);
    
    if(ArgQ < 4)
    {
        printf("Wrong number of Arguments.\n");
        return 1;
    }
    
    String_SetChars(& InputPath, ArgList[1]);
    String_SetChars(& OutputPath, ArgList[2]);
    String_SetChars(& StrPitch, ArgList[3]);
    
    String_SetChars(& StrVelocity, "100");
    String_SetChars(& StrFlags, "");
    String_SetChars(& StrOffset, "0");
    String_SetChars(& StrLength, "-1");
    
    if(ArgQ > 4)
        String_SetChars(& StrVelocity, ArgList[4]);
    if(ArgQ > 5)
        String_SetChars(& StrFlags, ArgList[5]);
    if(ArgQ > 6)
        String_SetChars(& StrOffset, ArgList[6]);
    if(ArgQ > 7)
        String_SetChars(& StrLength, ArgList[7]);
    
    number Fundamental = FreqFromPitch(& StrPitch);
    number Velocity    = CNumberStr(& StrVelocity);
    int    Length      = CIntStr(& StrLength);
    if(Fundamental < 50)
    {
        printf("Wrong pitch.\n");
        return 1;
    }
    
    /*
    printf("resampler (in)%s (out)%s (pitch)%s (vel)%s (flags)%s (offset)%s "
           "(length)%s\n",
        String_GetChars(& InputPath), String_GetChars(& OutputPath),
        String_GetChars(& StrPitch), String_GetChars(& StrVelocity), 
        String_GetChars(& StrFlags), String_GetChars(& StrOffset), 
        String_GetChars(& StrLength));
    */
    
    Wave InWave, OutWave;
    FWindow_T DyWin;
    PSOLAIterlyzer PAna;
    PSOLAItersizer PSyn;
    List_DataFrame DataList;
    number* HannWind = RCall(RAlloc, number)(2048);
    RNew(Wave, & InWave, & OutWave);
    RNew(PSOLAIterlyzer, & PAna);
    RNew(PSOLAItersizer, & PSyn);
    RNew(List_DataFrame, & DataList);
    
    RCall(CDSP2_GenHanning, number)(HannWind, 2048);
    RCall(Wave, Resize)(& OutWave, 100000);
    RCall(Wave, SetWindow)(& InWave, HannWind, 2048);
    RCall(Wave, SetWindow)(& OutWave, HannWind, 2048);
    
    if(! RCall(Wave, FromFile)(& InWave, & InputPath))
    {
        printf("Cannot read file!\n");
        return 1;
    }
    
    Length = Length * (number)InWave.SampleRate / 1000;
    if(Length < 0) Length = InWave.Size;
    
    int VOT = RCall(CSVP_VOTFromWave, number)(& InWave, 0, InWave.Size / 2);
    int Onset = RCall(CSVP_OnsetFromWave, number)
        (& InWave, 0.005, 0, InWave.Size);
    
    printf("Got VOT at %d.\n", VOT);
    printf("Got Onset at %d.\n", Onset);
    
    //Analysis process
    RCall(PSOLAIterlyzer, SetWave)(& PAna, & InWave);
    RCall(PSOLAIterlyzer, SetPosition)(& PAna, VOT + 2000);
    RCall(PSOLAIterlyzer, SetBound)(& PAna, VOT);
    
    if(! RCall(PSOLAIterlyzer, PreAnalysisTo)(& PAna, VOT + 4000))
    {
        printf("Preanalysis failed.\n");
        return 1;
    }
    RCall(PSOLAIterlyzer, IterNextTo)(& PAna, InWave.Size);
    RCall(PSOLAIterlyzer, PrevTo)(& PAna, Onset);
    RCall(List_DataFrame, FromWave)(& DataList, & InWave, & PAna.PulseList);
    
    
    int i;
    /*
    for(i = 0; i <= PAna.PulseList.Frames_Index; i ++)
    {
        int p = RCall(PSOLAIterlyzer, Fetch)(& PAna, i);
        printf("%f %f\n", (float)p / InWave.SampleRate
                        , (float)p / InWave.SampleRate);
    }*/
    
    //Pre-synthesis process
    OutWave.SampleRate = InWave.SampleRate;
    
    RCall(FWindow_T, Ctor)(& DyWin);
    RCall(FWindow, SetPara)(& DyWin, 30, 3000, 20);
    RCall(FWindow, SetFunc)(& DyWin, 
        _C(RFNL_Hanning_Size_Gnrc, _, number),
        _C(RFNL_Hanning_Valu_Gnrc, _, number));
    RCall(FWindow, Initialize)(& DyWin);
    
    RCall(PSOLAItersizer, SetWave)(& PSyn, & OutWave);
    RCall(PSOLAItersizer, SetWindow)(& PSyn, & DyWin);
    RCall(PSOLAItersizer, SetPosition)(& PSyn, 0);
    
    //Transposition process
    PMatch SorcPulse, LengthMatch;
    RNew(PMatch, & SorcPulse, & LengthMatch);
    for(i = 0; i <= PAna.PulseList.Frames_Index; i ++)
        PAna.PulseList.Frames[i] -= Onset;
    PMatchFromList_Int(& SorcPulse, & PAna.PulseList);
    int Fill = SorcPulse.X[1] - SorcPulse.X[0];
    i = 0;
    number p = SorcPulse.X[0];
    number Period = (number)InWave.SampleRate / Fundamental;
    while(p < VOT - Onset)
    {
        i = RCall(PMatch, Query)(& SorcPulse, p).LowerIndex;
        RCall(PSOLAItersizer, Add)(& PSyn, p, & DataList.Frames[i]);
        p += Fill;
    }
    
    RCall(PMatch, AddPair)(& LengthMatch, VOT - Onset, VOT - Onset);
    RCall(PMatch, AddPair)(& LengthMatch, Length,
        SorcPulse.X[SorcPulse.X_Index - 1]);
    while(p < Length)
    {
        Transition TransPulse;
        int p2 = RCall(PMatch, Query)(& LengthMatch, p).Y;
        TransPulse = RCall(PMatch, Query)(& SorcPulse, p2);
        i = TransPulse.LowerIndex;
        RCall(PSOLAItersizer, Add)(& PSyn, p, & DataList.Frames[i]);
        p += Period;
    }
    
    int Offset = (int)CNumberStr(& StrOffset) * InWave.SampleRate / 1000;
    //int OffIndex = RCall(PMatch, Query)(& SorcPulse, Offset).LowerIndex;
    RCall(PSOLAItersizer, RepositionFrom)(& PSyn, Onset - Offset);
    
    //Synthesis process
    RCall(PSOLAItersizer, IterNextTo)(& PSyn, InWave.Size);
    RCall(Wave, ToFile)(& OutWave, & OutputPath);
    
    printf("Generated.\n");
    
    RFree(HannWind);
    RDelete(& OutputPath, & InputPath, & StrPitch, & StrVelocity, & StrFlags,
        & InWave, & OutWave, & DyWin, & PAna, & PSyn, & DataList, & SorcPulse,
        & LengthMatch, & StrOffset, & StrLength);
}

