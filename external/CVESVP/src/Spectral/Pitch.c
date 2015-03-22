#include "Pitch.h"

#if 0
#include "_Pitch.rc"
#endif

#define First CSVP_PitchModel* This
#define _PMatch PMatch_Float_Float

RCtor(CSVP_PitchModel)
{
    RCall(_PMatch, Ctor)(& This -> HmncCurve);
    RCall(_PMatch, Ctor)(& This -> NoizCurve);
    RCall(_PMatch, Ctor)(& This -> PhseCurve);
    CSVP_PitchModel_SetDefault(This, CSVP_Gender_Female);
    
    RInit(CSVP_PitchModel);
}

RDtor(CSVP_PitchModel)
{
    RCall(_PMatch, Dtor)(& This -> HmncCurve);
    RCall(_PMatch, Dtor)(& This -> NoizCurve);
    RCall(_PMatch, Dtor)(& This -> PhseCurve);
}

void CSVP_PitchModel_From(First, CSVP_PitchModel* Sorc)
{
    This -> LDecay_F0 = Sorc -> LDecay_F0;
    This -> LDecay_Sensitivity = Sorc -> LDecay_Sensitivity;
    This -> LDecay_Center = Sorc -> LDecay_Center;
    This -> LDecay_Bound = Sorc -> LDecay_Bound;
    This -> HDecay_F0 = Sorc -> HDecay_F0;
    This -> HDecay_Sensitivity = Sorc -> HDecay_Sensitivity;
    This -> HDecay_Center = Sorc -> HDecay_Center;
    This -> HDecay_Bound = Sorc -> HDecay_Bound;
    RCall(_PMatch, From)(& This -> HmncCurve, & Sorc -> HmncCurve);
    RCall(_PMatch, From)(& This -> NoizCurve, & Sorc -> NoizCurve);
    RCall(_PMatch, From)(& This -> PhseCurve, & Sorc -> PhseCurve);
}

void CSVP_PitchModel_SetDefault(First, int Gender)
{
    //Currently Gender option is neglected.
    
    This -> LDecay_F0 = 240.0;
    This -> LDecay_Sensitivity = 0.02;
    This -> LDecay_Center = 4000.0;
    This -> LDecay_Bound = 9000.0;
    This -> HDecay_F0 = 450.0;
    This -> HDecay_Sensitivity = 0.002;
    This -> HDecay_Center = 2500.0;
    This -> HDecay_Bound = 6500.0;
    
    RCall(_PMatch, AddPair)(& This -> HmncCurve, 700.0, +0.0);
    RCall(_PMatch, AddPair)(& This -> HmncCurve, 360.0, +0.0);
    RCall(_PMatch, AddPair)(& This -> HmncCurve, 260.0, +0.0);
    RCall(_PMatch, AddPair)(& This -> HmncCurve, 150.0, -1.0);
    RCall(_PMatch, AddPair)(& This -> HmncCurve,   0.0, -2.0);
    
    RCall(_PMatch, AddPair)(& This -> NoizCurve, 700.0, -1.0);
    RCall(_PMatch, AddPair)(& This -> NoizCurve, 300.0, +1.0);
    RCall(_PMatch, AddPair)(& This -> NoizCurve, 200.0, +1.0);
    RCall(_PMatch, AddPair)(& This -> NoizCurve, 150.0, +0.5);
    RCall(_PMatch, AddPair)(& This -> NoizCurve,   0.0, -0.5);
    
    RCall(_PMatch, AddPair)(& This -> PhseCurve, 700.0,  1.7);
    RCall(_PMatch, AddPair)(& This -> PhseCurve, 360.0,  1.0);
    RCall(_PMatch, AddPair)(& This -> PhseCurve, 260.0,  1.0);
    RCall(_PMatch, AddPair)(& This -> PhseCurve, 200.0,  0.5);
    RCall(_PMatch, AddPair)(& This -> PhseCurve, 150.0,  0.2);
    RCall(_PMatch, AddPair)(& This -> PhseCurve,   0.0,  0.0);
}

CSVP_SpectralDecay CSVP_PitchModel_GetDecay(First, float F0)
{
    CSVP_SpectralDecay Ret;
    
    Ret.Center = 4000.0;
    Ret.Bound  = 10000.0;
    Ret.Intensity = 0.0;
    
    float Delta;
    if(F0 < This -> LDecay_F0)
    {
        Ret.Center = This -> LDecay_Center;
        Ret.Bound  = This -> LDecay_Bound;
        Delta = This -> LDecay_F0 - F0;
        Ret.Intensity = This -> LDecay_Sensitivity * Delta;
    }
    else if(F0 > This -> HDecay_F0)
    {
        Ret.Center = This -> HDecay_Center;
        Ret.Bound  = This -> HDecay_Bound;
        Delta = F0 - This -> HDecay_F0;
        Ret.Intensity = This -> HDecay_Sensitivity * Delta;
    }
    
    return Ret;
}

float CSVP_PitchModel_GetHmncAmp(First, float F0)
{
    return RCall(_PMatch, Query)(& This -> HmncCurve, F0).Y;
}

float CSVP_PitchModel_GetNoizAmp(First, float F0)
{
    return RCall(_PMatch, Query)(& This -> NoizCurve, F0).Y;
}

float CSVP_PitchModel_GetPhseCoh(First, float F0)
{
    return RCall(_PMatch, Query)(& This -> PhseCurve, F0).Y;
}

#undef  First

//-----
#include <RUtil2.h>

#define _RTAddress "Spectral/_Pitch.rc"

#define _ClassName _
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

