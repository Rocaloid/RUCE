#ifndef RUTIL2_STRING_H
#define RUTIL2_STRING_H

#include "../Core/OO.h"

RClass(String)
{
    RInherit(RObject);
    Array_Define(char, Data);
    int WordNum;
};

#define String_FromChars(name, chars) \
    String name; \
    do \
    { \
        String_Ctor(& name); \
        String_SetChars(& name, chars); \
    } while(0)

//Member Functions

void String_SetChars(String* This, const char* Chars);
void String_SetCharsN(String* This, const char* Chars, int WordNum);
char* String_GetChars(String* This);

void String_Copy(String* This, String* Sorc);
#define String_From(This, Sorc) String_Copy(This, Sorc)

int String_GetLength(String* This);
void String_AllocLength(String* This, int Length);
void String_AllocWordNum(String* This, int WordNum);
int String_GetWordNum(String* This);

char String_GetChar(String* This, int Index);
void String_SetChar(String* This, int Index, char Data);
void String_GetWord(String* This, String* Dest, int Index);
void String_SetWord(String* This, int Index, String* Data);

void String_Join(String* This, String* Sorc);
void String_JoinChars(String* This, char* Sorc);

int String_Equal(String* This, String* Sorc);
int String_EqualN(String* This, String* Sorc, int w);
int String_EqualChars(String* This, const char* Sorc);
int String_EqualNChars(String* This, const char* Sorc, int w);

int String_InStrFrom(String* This, String* Part, int From);
int String_InStr(String* This, String* Part);
int String_InStrRevFrom(String* This, String* Part, int From);
int String_InStrRev(String* This, String* Part);

//Abbreviations
#define InStrFrom    String_InStrFrom
#define InStr        String_InStr
#define InStrRevFrom String_InStrRefFrom
#define InStrRev     String_InStrRev

//String Utils
void Mid(String* Dest, String* Sorc, int From, int Count);
void MidFrom(String* Dest, String* Sorc, int From);
void Right(String* Dest, String* Sorc, int Count);
void Left(String* Dest, String* Sorc, int Count);

void UpperCase(String* Dest, String* Sorc);
void LowerCase(String* Dest, String* Sorc);

void Trim(String* Dest, String* Sorc);
void LTrim(String* Dest, String* Sorc);
void RTrim(String* Dest, String* Sorc);

int Wildcard_Match(String* Sorc, String* Wildcard);

#endif //RUTIL2_STRING_H

