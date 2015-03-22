#ifndef CDSP2_SPECTRUM_H
#define CDSP2_SPECTRUM_H

#include <RUtil2.h>

#define CDSP2_LinearMagn 0
#define CDSP2_LogMagn    1
#define CDSP2_dBMagn     2

#if 0
#include "_Spectrum.h"
#endif

#ifdef __CDSP2_Install
#define _RTAddress "CVEDSP2/Container/_Spectrum.h"
#else
#define _RTAddress "Container/_Spectrum.h"
#endif

#define _ClassName CDSP2_Spectrum_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

