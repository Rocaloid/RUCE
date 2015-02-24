#include "HNM.h"
#include "F0.h"
#include <stdlib.h>
#include <RFNL.h>

#if 0
#include "Iterator/_SinuGenericIterlyzer.rc"
#include "Iterator/_HNMItersizer.rc"
#endif

#include <RUtil2.h>

#define _RTAddress "Iterator/_SinuGenericIterlyzer.rc"

#define _ClassName CSVP_HNMIterlyzer_
#define _Attr 1

#define _T4 HNMList
#define _T3 HNMFrame
#define _T2 CSVP_HNMIterlyzer
#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T4 HNMList
#define _T3 HNMFrame
#define _T2 CSVP_HNMIterlyzer
#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#define _RTAddress "Iterator/_HNMItersizer.rc"

#define _ClassName CSVP_HNMItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

