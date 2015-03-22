#ifndef CSVP_PITCH_H
#define CSVP_PITCH_H

#if 0
#include "_Pitch.h"
#endif

#include "Spectral.h"

#define First CSVP_PitchModel* This

RClass(CSVP_PitchModel)
{
    RInherit(RObject);
    
    //Public
    float LDecay_F0;
    float LDecay_Sensitivity; // -log amp per Hz of F0
    float LDecay_Center;
    float LDecay_Bound;
    float HDecay_F0;
    float HDecay_Sensitivity;
    float HDecay_Center;
    float HDecay_Bound;
    
    PMatch_Float_Float HmncCurve; //Amplitude of harmonic component with Pitch
    PMatch_Float_Float NoizCurve; //Amplitude of noise component with Pitch
    PMatch_Float_Float PhseCurve; //Phase coherence with Pitch
};

void CSVP_PitchModel_From(First, CSVP_PitchModel* Sorc);
void CSVP_PitchModel_SetDefault(First, int Gender);

CSVP_SpectralDecay CSVP_PitchModel_GetDecay(First, float F0);
float CSVP_PitchModel_GetHmncAmp(First, float F0);
float CSVP_PitchModel_GetNoizAmp(First, float F0);
float CSVP_PitchModel_GetPhseCoh(First, float F0);

#undef  First

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Spectral/_Pitch.h"
#else
#define _RTAddress "Spectral/_Pitch.h"
#endif

#define _ClassName _
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

