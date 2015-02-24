#include "HNM.h"
#include "F0.h"
#include "Phase.h"
#include <RUtil2.h>
#include <RFNL.h>
#include <stdlib.h>

#if 0
#include "Iterator/_SinusoidalBase.rc"
#include "Iterator/_SinuGenericIterlyzer.rc"
#include "Iterator/_SinusoidItersizer.rc"
#endif

#define ANALSIZE 2048
#define ANALHOP 256

#define _RTAddress "Iterator/_SinusoidalBase.rc"

#define _ClassName CSVP_SinusoidalBase_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#define _RTAddress "Iterator/_SinuGenericIterlyzer.rc"

#define _ClassName CSVP_SinusoidIterlyzer_
#define _Attr 1

#define _T4 SinuList
#define _T3 Sinusoid
#define _T2 CSVP_SinusoidIterlyzer
#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T4 SinuList
#define _T3 Sinusoid
#define _T2 CSVP_SinusoidIterlyzer
#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#define _RTAddress "Iterator/_SinusoidItersizer.rc"

#define _ClassName CSVP_SinusoidItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

