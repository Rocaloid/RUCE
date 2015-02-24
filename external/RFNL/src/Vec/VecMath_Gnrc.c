//Generic Vectorized Math implementation

#include "Vec.h"
#include <RUtil2.h>

#define __RFNL_Unroll_Arg1Op4_Gnrc(Operator) \
    int i; \
    for(i = 0; i < Size - 3; i += 4) \
    { \
        Dest[i + 0] = Operator(Sorc[i + 0]); \
        Dest[i + 1] = Operator(Sorc[i + 1]); \
        Dest[i + 2] = Operator(Sorc[i + 2]); \
        Dest[i + 3] = Operator(Sorc[i + 3]); \
    } \
    for(; i < Size; i ++) \
        Dest[i + 0] = Operator(Sorc[i + 0])

#if 0
#include "_VecMath_Gnrc.rc"
#endif

#undef _RTAddress
#ifdef __RFNL_Install
#define _RTAddress "RFNL/Vec/_VecMath_Gnrc.rc"
#else
#define _RTAddress "Vec/_VecMath_Gnrc.rc"
#endif

#define _ClassName
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

