#ifndef CSVP_SPECTRAL_H
#define CSVP_SPECTRAL_H

#if 0
#include "_Spectral.h"
#endif

#include "../Structure/HNMFrame.h"
#include <RUtil2.h>

#define CSVP_Gender_Male   0
#define CSVP_Gender_Female 1

typedef struct
{
    float Center;
    float Bound;
    float Intensity;
} CSVP_SpectralDecay;

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Spectral/_Spectral.h"
#else
#define _RTAddress "Spectral/_Spectral.h"
#endif

#define _ClassName _
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

