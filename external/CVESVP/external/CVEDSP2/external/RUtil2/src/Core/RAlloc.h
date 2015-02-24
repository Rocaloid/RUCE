#ifndef RUTIL2_RALLOC_H
#define RUTIL2_RALLOC_H

#include <memory.h>

#include "OO.h"

void* RAlloc(int Size);
void* RAlign(int Align, int Size);

#if defined(__MINGW32__)
#define _aligned_malloc __mingw_aligned_malloc
#define _aligned_free  __mingw_aligned_free
#define memalign(align, size) _aligned_malloc(size, align)
#endif

#define RFree(...) __RFree(__VA_ARGS__, (void*)(- 1))
void __RFree(void* a, ...);

#define RAlloc_Class(Name, Size) \
    (Name*)__RAlloc_Class(Size, sizeof(Name), _C(__ClassID_, Name, __));
void* __RAlloc_Class(int Size, int UnitSize, int ClassID);

#if 0
#include "_RAlloc.h"
#endif
#ifdef __RUtil2_Install
#define _RTAddress "RUtil2/Core/_RAlloc.h"
#else
#define _RTAddress "Core/_RAlloc.h"
#endif

#define _ClassName
#define _Attr 1

#include "Include_T1AllTypes.h"

#endif //RUTIL2_RALLOC_H

