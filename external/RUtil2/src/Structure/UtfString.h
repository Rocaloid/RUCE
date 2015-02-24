#ifndef RUTIL2_UTFSTRING_H
#define RUTIL2_UTFSTRING_H

#include <unistd.h>
#include <string.h>
#include "../Core/OO.h"

RInline int strlen_utf8(char* s) 
{
    return strlen(s);
}

RInline char* strcpy_utf8(char* Dest, char* Sorc)
{
    return strcpy(Dest, Sorc);
}

RInline int strcmp_utf8(char* a, char* b)
{
    return strcmp(a, b);
}

RInline char* strcat_utf8(char* Dest, char* Sorc)
{
    return strcat(Dest, Sorc);
}

int strnum_utf8(const char* s);
int strnnum_utf8(const char* s, int n);
int strwlen_utf8(const char* s, int w);

int strwcmp_utf8(const char* a, const char* b, int w);

char* strncpy_utf8(char* __restrict Dest, 
                   const char* __restrict Sorc, 
                   int n);

char* strncat_utf8(char* __restrict Dest, 
                   const char* __restrict Sorc, 
                   int n);

char* next_char_safe_utf8(const char* p, const char* StrEnd);
char* next_char_utf8(const char* p);
char* prev_char_safe_utf8(const char* StrBegin, const char* p);
char* prev_char_utf8(const char* p);

int GetWordLength_Utf8(const char Head);

#endif //RUTIL2_UTFSTRING_INTERN_H
