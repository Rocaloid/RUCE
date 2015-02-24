#ifndef CDSP2_GAIN_H
#define CDSP2_GAIN_H

#include "Container/Wave.h"
#include <RUtil2.h>

#if 0
#include "Iterator/_GainIterfector.h"
#include "Iterator/_NormIterfector.h"
#endif

#ifdef __CDSP2_Install
#define _RTAddress "CVEDSP2/Iterator/_GainIterfector.h"
#else
#define _RTAddress "Iterator/_GainIterfector.h"
#endif

#define _ClassName CDSP2_GainIterfector_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#ifdef __CDSP2_Install
#define _RTAddress "CVEDSP2/Iterator/_NormIterfector.h"
#else
#define _RTAddress "Iterator/_NormIterfector.h"
#endif

#define _ClassName CDSP2_NormIterfector_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

