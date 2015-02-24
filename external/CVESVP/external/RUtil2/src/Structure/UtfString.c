#include "UtfString.h" 

#include <string.h>
#include <memory.h>
#include "../Core/OO.h"

static const int Utf8_Skip_Data[256] = 
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

#define _utf8_NCopy_Core(Dest, Sorc, n)                                       \
    {                                                                         \
        int U8Size;                                                        \
        while(* Sorc && (U8Size = Utf8_Skip_Data[(UChar)(* Sorc)]) < n)      \
        {                                                                     \
            n -= U8Size;                                                      \
            memcpy(Dest, Sorc, U8Size);                                       \
            Dest += U8Size;                                                   \
            Sorc += U8Size;                                                   \
        }                                                                     \
        * Dest = '\0';                                                        \
    } (void)0

int strnum_utf8(const char* s)
{
    int Len;
    
    for (Len = 0; * s; ++Len)
        s += Utf8_Skip_Data[(UChar)(* s)];
    
    return Len;
}

int strnnum_utf8(const char* s, int n)
{
    int Len;
    const char* s_end = s + n;
    
    for (Len = 0; * s && s < s_end; ++Len)
        s += Utf8_Skip_Data[(UChar)(* s)];
    
    if(s > s_end)
        --Len;
    
    return Len;
}

int strwlen_utf8(const char* s, int w)
{
    int Len;
    const char* Curr = s;
    
    for (Len = 0; Len < w && (* Curr); ++Len)
        Curr += Utf8_Skip_Data[(UChar)(* s)];
    
    return Curr - s;
}

char* strncpy_utf8(char* __restrict Dest, 
                   const char* __restrict Sorc, 
                   int n)
{
    char* Dest_Raw = Dest;
    
    RAssert(n);

    /* NOTE: currently we don't attempt to deal with invalid utf8 chars */
    _utf8_NCopy_Core(Dest, Sorc, n);
    
    return (char *)Dest_Raw;
}

char* strwcpy_utf8(char* __restrict Dest, 
                   const char* __restrict Sorc, 
                   int n)
{
    char* Dest_Raw = Dest;
    
    RAssert(n);

    /* NOTE: currently we don't attempt to deal with invalid utf8 chars */
    _utf8_NCopy_Core(Dest, Sorc, n);
    
    return (char *)Dest_Raw;
}

int strwcmp_utf8(const char* a, const char* b, int w)
{
    char tmp;
    
    for(int i = 0; i < w; ++i)
    {
        int WLenA = Utf8_Skip_Data[(UChar)(a[0])];
        for(int j = 0; j < WLenA; ++j)
        {
            if((tmp = a[j] - b[j]))
                return tmp;
        }
        a = next_char_utf8(a);
        b = next_char_utf8(b);
    }
    
    return 0;
}

char* strncat_utf8(char* __restrict Dest, 
                   const char* __restrict Sorc, 
                   int n)
{
    while(* Dest && n > 0)
    {
        ++Dest;
        --n;
    }
    
    _utf8_NCopy_Core(Dest, Sorc, n);

    return (char *)Dest;
}

char* next_char_safe_utf8(const char* p, const char* StrEnd)
{
    if(Likely(* p))
    {
        if(Likely(StrEnd))
            for(++p; p < StrEnd && (* p & 0xc0) == 0x80; ++p);
        else
            for(++p; (* p & 0xc0) == 0x80; ++p);
    }
    return (p > StrEnd) ? NULL : (char* )p;
}

char* next_char_utf8(const char* p)
{
    if(Likely(* p))
    {
        p += Utf8_Skip_Data[(UChar)(* p)];
    }
    return (char*)p;
}

char* prev_char_safe_utf8(const char* p, const char* StrBegin)
{
    for(--p; p >= StrBegin; --p)
    {
        if ((* p & 0xc0) != 0x80)
            return (char*)p;
    }
    
    return NULL;
}

char* prev_char_utf8(const char* p)
{
    while(1)
    {
        --p;
        if ((* p & 0xc0) != 0x80)
            return (char*)p;
    }
}

int GetWordLength_Utf8(const char Head)
{
    return Utf8_Skip_Data[(UChar)Head];
}
