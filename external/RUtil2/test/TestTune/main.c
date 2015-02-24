#include <stdio.h>

#include "Core/OO.h"
#include "Structure/String.h"
#include "Misc/Tune.h"

int Tune_PitSPN_Test(int L, int R)
{
    int Ret = 0;
    String o;
    String_Ctor(& o);
    for(int i = L; i < R; ++i)
    {
        Tune_PitchToSPN_Int(i, & o);
        if(Tune_SPNToPitch_Int(& o) != i)
        {
            printf("[Error] Test is not passed @ i = %d.\n", i);
            Ret = 1;
        }
    }
    
    for(float i = (float)L; i < (float)R; i += 0.3)
    {
        Tune_PitchToSPN_Float(i, & o);
        if(Tune_SPNToPitch_Float(& o) != round(i))
        {
            printf("[Error] Test is not passed @ i = %f.\n", i);
            Ret = 2;
        }
    }
    
    RDelete(& o);
    
    return Ret;
}

int Tune_PitFreq_Test(int L, int R)
{
    int Ret = 0;
    for(float i = L + 0.1f; i < R; i += 0.3)
    {
        if(fabsf(Tune_PitchToFreq_Float(Tune_FreqToPitch_Float(i)) - i) >= 0.1)
        {
            printf("[Error] Test is not passed @ i = %f.\n", i);
            Ret = 3;
        }
    }
    
    for(double i = L + 0.1f; i < R; i += 0.3)
    {
        if(fabs(Tune_PitchToFreq_Double(
            Tune_FreqToPitch_Double(i)) - i) >= 0.5)
        {
            printf("[Error] Test is not passed @ i = %lf.\n", i);
            Ret = 4;
        }
    }
    
    return Ret;
}

int main()
{
    int Ret = 0;
    
    Tune_SetPitchNameMode(Tune_AlwaysRising);
    if((Ret = Tune_PitSPN_Test(-2000, 2000)))
        goto StopTest;
    
    Tune_SetPitchNameMode(Tune_AlwaysFalling);
    if((Ret = Tune_PitSPN_Test(-2000, 2000)))
        goto StopTest;
    
    Tune_SetPitchNameMode(Tune_Mixing);
    if((Ret = Tune_PitSPN_Test(-2000, 2000)))
        goto StopTest;
    
    if((Ret = Tune_PitFreq_Test(0, 50000)))
        goto StopTest;
    
StopTest:
    return Ret;
}