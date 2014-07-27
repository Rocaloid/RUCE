#include "Roto.h"
#include "IO.h"
#include "GetPara.h"
#include <CVESVP.h>

int main(int ArgN, char** Arg)
{
    RUCE_ResamplerPara Para;
    RUCE_ResamplerPara_Ctor(& Para);
    
    int i;
    for(i = 0; i < ArgN; i ++)
    {
        printf("%s\n", Arg[i]);
    }
    
    RUCE_ParsePara(& Para, ArgN, Arg);
    /*
    String Input, Output;
    float InvarLeft, InvarRight;
    float LenRequire, FixedLength;
    float Velocity, Volume, Modulation;
    PMatch_Float_Float Freq;
    */
    printf("Input: %s, Output: %s\n",
        String_GetChars(& Para.Input), String_GetChars(& Para.Output));
    printf("InvarLeft: %f, InvarRight: %f\n", Para.InvarLeft, Para.InvarRight);
    printf("LenRequire: %f, FixedLength: %f\n", Para.LenRequire,
        Para.FixedLength);
    printf("Velocity: %f, Modulation: %f\n", Para.Velocity, Para.Modulation);
    PMatch_Float_Float_Print(& Para.Freq);
    
    RDelete(& Para);
    return 0;
}

