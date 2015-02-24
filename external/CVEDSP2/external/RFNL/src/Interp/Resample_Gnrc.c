//Generic Resampling Implementation

#include "Interp.h"
#include <RUtil2.h>

#if 0
#include "_Resample_Gnrc.rc"
#endif

#define _RTAddress "Interp/_Resample_Gnrc.rc"

#define _ClassName
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#undef _RTAddress

