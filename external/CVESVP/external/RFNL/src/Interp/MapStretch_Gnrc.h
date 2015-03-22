//Generic Series Mapping, Compression & Expansion Header

#include <RUtil2.h>

#if 0
#include "_MapStretch_Gnrc.h"
#endif

#ifdef __RFNL_Install
#define _RTAddress "RFNL/Interp/_MapStretch_Gnrc.h"
#else
#define _RTAddress "Interp/_MapStretch_Gnrc.h"
#endif

//_T1 for control points
//_T2 for amplitude
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

#undef _RTAddress

