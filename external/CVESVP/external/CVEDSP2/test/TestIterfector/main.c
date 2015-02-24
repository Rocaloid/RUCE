#include <RUtil2.h>
#include "WaveUtils.h"
#include "Gain.h"
#include "Container/Wave.h"

#define Wave CDSP2_Wave_Float
#define GainIterfector CDSP2_GainIterfector_Float
#define NormIterfector CDSP2_NormIterfector_Float

int main(void)
{
    String Path;
    Wave InWave, OutWave, EWave;
    String_Ctor(& Path);
    RCall(Wave, Ctor)(& InWave);
    RCall(Wave, Ctor)(& OutWave);
    RCall(Wave, Ctor)(& EWave);
    String_SetChars(& Path, "/tmp/out.wav");
    if(! RCall(Wave, FromFile)(& InWave, & Path))
        fprintf(stderr, "Cannot open wave.\n");
    RCall(Wave, Resize)(& OutWave, InWave.Size);
    
    CDSP2_EnergyCurveFromWaveDB_Float(& EWave, & InWave, 1024);
    int i;
    
    for(i = 0; i < InWave.Size; i += 128)
    {
        printf("%d %f\n", i + 512, EWave.Data[i]);
        //printf("%d %f\n", i, CDSP2_MeanEnergyFromWaveDB_Float
        //    (& InWave, i - 512, 1024));
    }
    OutWave.SampleRate = InWave.SampleRate;
    
    /*
    GainIterfector Amplifier;
    RCall(GainIterfector, Ctor)(& Amplifier);
    RCall(GainIterfector, SetWave)(& Amplifier, & InWave);
    RCall(GainIterfector, SetOutWave)(& Amplifier, & OutWave);
    RCall(GainIterfector, SetPosition)(& Amplifier, 10000);
    
    //RCall(GainIterfector, Add)(& Amplifier, 0, 0);
    RCall(GainIterfector, Add)(& Amplifier, 1.9, 14000);
    RCall(GainIterfector, Add)(& Amplifier, 2, 15000);
    RCall(GainIterfector, Add)(& Amplifier, 0.5, 10000);
    //RCall(GainIterfector, Add)(& Amplifier, 1.5, 30000);
    
    RCall(GainIterfector, PrevTo)(& Amplifier, 0);
    RCall(GainIterfector, IterNextTo)(& Amplifier, InWave.Size);*/
    
    NormIterfector Normalizer;
    RCall(NormIterfector, Ctor)(& Normalizer);
    RCall(NormIterfector, SetWave)(& Normalizer, & InWave);
    RCall(NormIterfector, SetOutWave)(& Normalizer, & OutWave);
    RCall(NormIterfector, SetEnergyWave)(& Normalizer, & EWave);
    RCall(NormIterfector, SetPosition)(& Normalizer, 10000);
    RCall(NormIterfector, SetGain)(& Normalizer, 3);
    RCall(NormIterfector, SetIntensity)(& Normalizer, -50);
    
    RCall(NormIterfector, PrevTo)(& Normalizer, 0);
    RCall(NormIterfector, IterNextTo)(& Normalizer, InWave.Size);
    
    String_SetChars(& Path, "/tmp/out.wav");
    RCall(Wave, ToFile)(& OutWave, & Path);
    RDelete(& InWave, & OutWave, & EWave, & Path, & Normalizer);
    return 0;
}

