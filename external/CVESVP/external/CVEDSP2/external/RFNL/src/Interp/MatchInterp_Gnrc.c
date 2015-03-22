//Generic PMatch Interpolation Implementation

#include "../Vec/Vec.h"
#include "Interp.h"
#include <RUtil2.h>

#if 0
#include "_MatchInterp_Gnrc.rc"
#endif

#define _RTAddress "Interp/_MatchInterp_Gnrc.rc"

#define _ClassName
#define _Attr 2

#define _T1 Float
#define _T2 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Float
#define _T2 Double
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#define _T2 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#define _T2 Double
#include <RUtil2/Core/RTemplate.h>

