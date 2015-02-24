#if defined(__APPLE__)
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <stdarg.h>
#include "RAlloc.h"
#include "OO.h"

void* RAlloc(int Size)
{
    void* ret = malloc(Size);
    return ret;
}

void* RAlign(int Align, int Size)
{
#if defined(__APPLE__) || defined(__MINGW32__) || defined(_WIN64)
    if(Size % Align)
    {
        return malloc(Size);
    }
    else
    {
        return malloc(Align * (Size / Align + 1));
    }

#else
    return memalign(Align, Size);
#endif
}

void __RFree(void* a, ...)
{
    void* Ptr;
    va_list Args;
    va_start(Args, a);

    //First
    free(a);
    while(1)
    {
        Ptr = va_arg(Args, void*);
        //End
        if(Ptr == (void *)-1)
            break;
        free(Ptr);
    }

    va_end(Args);

}

void* __RAlloc_Class(int Size, int UnitSize, int ClassID)
{
    return malloc(Size * UnitSize);
}

#if 0
#include "_RAlloc.rc"
#endif
#define _RTAddress "Core/_RAlloc.rc"

#define _ClassName
#define _Attr 1

#include "Include_T1AllTypes.h"

