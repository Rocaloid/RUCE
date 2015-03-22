//Vectorization Header

#ifndef RFNL_VEC_H
#define RFNL_VEC_H

#include <RUtil2.h>

#if 0
#include "_VecBasic_Gnrc.h"
#include "_VecMath_Gnrc.h"
#endif

#ifdef __RFNL_Install
#define _RTAddress "RFNL/Vec/_VecBasic_Gnrc.h"
#else
#define _RTAddress "Vec/_VecBasic_Gnrc.h"
#endif

#define _ClassName
#define _Attr 1

#include <RUtil2/Core/Include_T1AllTypes.h>

#undef _RTAddress
#ifdef __RFNL_Install
#define _RTAddress "RFNL/Vec/_VecMath_Gnrc.h"
#else
#define _RTAddress "Vec/_VecMath_Gnrc.h"
#endif

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

