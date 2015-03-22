#ifndef CDSP2_INFWAVE_H
#define CDSP2_INFWAVE_H

#include "Wave.h"
#include <RUtil2.h>

#if 0
#include "_InfWave.h"
#endif

#ifdef __CDSP2_Install
#define _RTAddress "CVEDSP2/Container/_InfWave.h"
#else
#define _RTAddress "Container/_InfWave.h"
#endif

#define _ClassName CDSP2_InfWave_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

