//Generic Template Basic Vectorization implementation

#include "Vec.h"
#include <RUtil2.h>

#define __RFNL_Unroll_Op4_Gnrc(Operator) \
    int i; \
    for(i = 0; i < Size - 3; i += 4) \
    { \
        Dest[i + 0] = Operator(Sorc1[i + 0], Sorc2[i + 0]); \
        Dest[i + 1] = Operator(Sorc1[i + 1], Sorc2[i + 1]); \
        Dest[i + 2] = Operator(Sorc1[i + 2], Sorc2[i + 2]); \
        Dest[i + 3] = Operator(Sorc1[i + 3], Sorc2[i + 3]); \
    } \
    for(; i < Size; i ++) \
        Dest[i + 0] = Operator(Sorc1[i + 0], Sorc2[i + 0])

#define __RFNL_Unroll_COp4_Gnrc(Operator) \
    int i; \
    for(i = 0; i < Size - 3; i += 4) \
    { \
        Dest[i + 0] = Operator(Sorc1[i + 0], Sorc2); \
        Dest[i + 1] = Operator(Sorc1[i + 1], Sorc2); \
        Dest[i + 2] = Operator(Sorc1[i + 2], Sorc2); \
        Dest[i + 3] = Operator(Sorc1[i + 3], Sorc2); \
    } \
    for(; i < Size; i ++) \
        Dest[i + 0] = Operator(Sorc1[i + 0], Sorc2)

#if 0
#include "_VecBasic_Gnrc.rc"
#endif

#ifdef __RFNL_Install
#define _RTAddress "RFNL/Vec/_VecBasic_Gnrc.rc"
#else
#define _RTAddress "Vec/_VecBasic_Gnrc.rc"
#endif

#define _ClassName
#define _Attr 1

#include <RUtil2/Core/Include_T1AllTypes.h>

