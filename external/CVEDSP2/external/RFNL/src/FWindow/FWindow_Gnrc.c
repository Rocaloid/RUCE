//Generic Fast Window Generator Implementation

#define __RFNL_Unroll_Window4_Gnrc(Func) \
    int i; \
    for(i = 0; i < Size -3; i += 4) \
    { \
        Dest[i + 0] = Sorc[i + 0] * Func(i + 0); \
        Dest[i + 1] = Sorc[i + 1] * Func(i + 1); \
        Dest[i + 2] = Sorc[i + 2] * Func(i + 2); \
        Dest[i + 3] = Sorc[i + 3] * Func(i + 3); \
    } \
    for(; i < Size; i ++) \
        Dest[i + 0] = Sorc[i + 0] * Func(i + 0)

#define __RFNL_Unroll_CWindow4_Gnrc(Func) \
    int i; \
    for(i = 0; i < Size -3; i += 4) \
    { \
        Dest[i + 0] = Func(i + 0); \
        Dest[i + 1] = Func(i + 1); \
        Dest[i + 2] = Func(i + 2); \
        Dest[i + 3] = Func(i + 3); \
    } \
    for(; i < Size; i ++) \
        Dest[i + 0] = Func(i + 0)

#include "FWindow.h"
#include "../Vec/Vec.h"
#include <RUtil2.h>

#if 0
#include "_FWindow_Gnrc.rc"
#endif

#define _RTAddress "FWindow/_FWindow_Gnrc.rc"

#define _ClassName RFNL_FWindow_Gnrc_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

