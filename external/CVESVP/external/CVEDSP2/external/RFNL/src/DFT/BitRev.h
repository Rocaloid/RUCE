//BitRev is the preparation function for FFTs.

#ifndef RFNL_BITREV_H
#define RFNL_BITREV_H

#include <RUtil2.h>

#if 0
#include "_BitRev_Gnrc.h"
#endif
#ifdef __RFNL_Install
#define _RTAddress "RFNL/DFT/_BitRev_Gnrc.h"
#else
#define _RTAddress "DFT/_BitRev_Gnrc.h"
#endif

#define _ClassName
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

