#include "Synth.h"

static void StretchConsonant(Wave* Dest, Wave* Sorc, int VOT,
    int SorcOrigin, int DestOrigin)
{
    #define NAnch 4
    int SorcAnch[NAnch];
    int DestAnch[NAnch];
    int i;
    int DestLen = VOT - DestOrigin;
    int SorcLen = VOT - SorcOrigin;
    for(i = 0; i < NAnch; i ++)
    {
        SorcAnch[i] = SorcOrigin + (SorcLen / 2 + SorcLen * i) / NAnch;
        DestAnch[i] = DestOrigin + (DestLen / 2 + DestLen * i) / NAnch;
    }
    int FirstAnch = 0;
    for(i = 0; i < NAnch; i ++)
        if(DestAnch[i] > 0)
        {
            FirstAnch = i;
            break;
        }
    if(DestAnch[FirstAnch] <= 0) return;
    /*
    ] 1|  2  |  3  |  4  | 5
    ]    |    |    |    |
    ]-------------------------------
    */
    RCall(Wave, Resize)(Dest, Sorc -> Size);
    Real* Temp = RCall(RAlloc, Real)(Sorc -> Size);
    RCall(CDSP2_VSet, Real)(Temp, 0, Sorc -> Size);
    // Region 1
    RCall(Wave, Read)(Sorc, Temp, SorcAnch[FirstAnch] - DestAnch[FirstAnch],
        DestAnch[FirstAnch]);
    RCall(CDSP2_VCopy, Real)(Dest -> Data, Temp, DestAnch[FirstAnch]);
    
    // Region 2 to 4
    for(i = FirstAnch; i < NAnch - 1; i ++)
    {
        //For each anchor point
        int j;
        for(j = DestAnch[i]; j < DestAnch[i + 1]; j ++)
        {
            Real R = (Real)(j - DestAnch[i]) / (DestAnch[i + 1] - DestAnch[i]);
            Dest -> Data[j] = Sorc -> Data[j - DestAnch[i] + SorcAnch[i]]
                * (1.0 - R)
                + Sorc -> Data[j - DestAnch[i + 1] + SorcAnch[i + 1]]
                * R;
        }
    }
    
    // Region 5
    RCall(Wave, Read)(Sorc, Temp, SorcAnch[NAnch - 1], Sorc -> Size -
        SorcAnch[NAnch - 1]);
    RCall(CDSP2_VCopy, Real)(Dest -> Data + DestAnch[NAnch - 1],
        Temp, Sorc -> Size - DestAnch[NAnch - 1]);
    
    RFree(Temp);
}

#define max(a, b) ((a) > (b) ? (a) : (b))
int RUCE_UnvoicedSynth(Wave* Dest, RUCE_Note* SorcNote, RUCE_DB_Entry* SorcDB)
{
    int SampleRate = Dest -> SampleRate;
    int DurSample = SampleRate * SorcNote -> CParamSet.DurConsonant;
    int VOTSample = SampleRate * SorcDB -> VOT;
    int Shift = max(DurSample, VOTSample) - VOTSample;
    int i;
    
    Wave Sorc;
    RCall(Wave, CtorSize)(& Sorc, SorcDB -> WaveSize + Shift);
    RCall(CDSP2_VSet, Real)(Sorc.Data, 0, Shift);
    for(i = 0; i < SorcDB -> WaveSize; i ++)
        Sorc.Data[i + Shift] = SorcDB -> ApWave[i];
    
    if(VOTSample > DurSample)
        StretchConsonant(Dest, & Sorc, VOTSample, Shift, 0);
    else
        StretchConsonant(Dest, & Sorc, DurSample, Shift, VOTSample - DurSample);
    
    RCall(Wave, Dtor)(& Sorc);
    return 1;
}

