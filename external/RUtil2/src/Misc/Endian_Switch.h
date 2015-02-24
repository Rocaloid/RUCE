#ifndef ENDIAN_SWITCH_H
#define ENDIAN_SWITCH_H

#include "../Core/OO.h"

#if 0
#include "_Endian_Switch.h"
#endif
#ifdef __RUtil2_Install
#define _RTAddress "RUtil2/Misc/_Endian_Switch.h"
#else
#define _RTAddress "Misc/_Endian_Switch.h"
#endif

#define _ClassName
#define _Attr 1

#define _T1 Float
#include "../Core/RTemplate.h"

#define _T1 Double
#include "../Core/RTemplate.h"

#define _T1 Int
#include "../Core/RTemplate.h"

#define _T1 UInt
#include "../Core/RTemplate.h"

#define _T1 Short
#include "../Core/RTemplate.h"

#define _T1 UShort
#include "../Core/RTemplate.h"

#define _T1 Int16
#include "../Core/RTemplate.h"

#define _T1 UInt16
#include "../Core/RTemplate.h"

#define _T1 Int32
#include "../Core/RTemplate.h"

#define _T1 UInt32
#include "../Core/RTemplate.h"

#define _T1 Int64
#include "../Core/RTemplate.h"

#define _T1 UInt64
#include "../Core/RTemplate.h"

#endif // ENDIAN_SWITCH_H
