//Generic Fast Window Generator Header

#include "../Interp/Interp.h"
#include <RUtil2.h>

#if 0
#include "_FWindow_Gnrc.h"
#endif

#ifdef __RFNL_Install
#define _RTAddress "RFNL/FWindow/_FWindow_Gnrc.h"
#else
#define _RTAddress "FWindow/_FWindow_Gnrc.h"
#endif

#define _ClassName RFNL_FWindow_Gnrc_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#undef _RTAddress

