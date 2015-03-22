//Fast Math Header

#ifndef RFNL_FMATH_H
#define RFNL_FMATH_H

#include <RUtil2.h>

#define RFNL_TableSize 65536
#ifndef M_PI
#define M_PI 3.14159265358979323846264f
#endif

#if 0
#include "_FMath_Comm.h"
#include "_FMath_Gnrc.h"
#endif

//Common
#ifdef __RFNL_Install
#define _RTAddress "RFNL/FMath/_FMath_Comm.h"
#else
#define _RTAddress "FMath/_FMath_Comm.h"
#endif

#define _ClassName
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//Generic
#undef _RTAddress
#ifdef __RFNL_Install
#define _RTAddress "RFNL/FMath/_FMath_Gnrc.h"
#else
#define _RTAddress "FMath/_FMath_Gnrc.h"
#endif

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

