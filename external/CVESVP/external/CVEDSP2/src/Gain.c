#include "Gain.h"
#include "WaveUtils.h"
#include <math.h>

#if 0
#include "Iterator/_GainIterfector.rc"
#include "Iterator/_NormIterfector.rc"
#endif

#include <RUtil2.h>

#define _RTAddress "Iterator/_GainIterfector.rc"
#define _ClassName CDSP2_GainIterfector_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#define _RTAddress "Iterator/_NormIterfector.rc"
#define _ClassName CDSP2_NormIterfector_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

