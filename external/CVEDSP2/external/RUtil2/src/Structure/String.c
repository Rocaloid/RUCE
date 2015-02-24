#include <string.h>
#include "String.h"
#include "UtfString.h"
#include "Array.h"
#include "../Core/OO.h"

RCtor(String)
{
    Array_Ctor(char, This -> Data);
    This -> WordNum = 0;
    RInit(String);
}

RDtor(String)
{
    Array_Dtor(char, This -> Data);
}

void String_AllocWordNum(String* This, int WordNum)
{
    String_AllocLength(This, WordNum * 6);
}

void String_SetChars(String* This, const char* Chars)
{
    int Len = strlen(Chars);
    Array_Resize(char, This -> Data, Len);
    memcpy(This -> Data, Chars, Len);
    This -> Data_Index = Len - 1;
    This -> WordNum = strnum_utf8(Chars);
}

void String_SetCharsN(String* This, const char* Chars, int WordNum)
{
    const char* Chars_End = Chars;
    int i;
    for(i = 0; i < WordNum && * Chars_End; ++ i)
        Chars_End = next_char_utf8(Chars_End);
    
    int DataLength = Chars_End - Chars;
    
    String_AllocLength(This, DataLength);
    
    memcpy(This -> Data, Chars, DataLength);
    
    This -> Data_Index = DataLength - 1;
    This -> WordNum = i;
}

char* String_GetChars(String* This)
{
    Array_Push(char, This -> Data, 0);
    This -> Data_Index --;
    return This -> Data;
}

void String_Copy(String* This, String* Sorc)
{
    Array_Resize(char, This -> Data, Sorc -> Data_Index + 1);
    memcpy(This -> Data, Sorc -> Data, Sorc -> Data_Index + 1);
    This -> Data_Index = Sorc -> Data_Index;
    This -> WordNum = Sorc -> WordNum;
}

void String_AllocLength(String* This, int Length)
{
    Array_Resize(char, This -> Data, Length);
}

int String_GetLength(String* This)
{
    return This -> Data_Index + 1;
}

int String_GetWordNum(String* This)
{
    return This -> WordNum;
}

char String_GetChar(String* This, int Index)
{
    return This -> Data[Index];
}

void String_SetChar(String* This, int Index, char Data)
{
    This -> Data[Index] = Data;
}

void String_GetWord(String* This, String* Dest, int Index)
{
    const char* Curr = This -> Data;
    for(int i = 0; i < Index; ++ i, Curr = next_char_utf8(Curr))
        RAssert(* Curr);
    int WordLength = GetWordLength_Utf8(Curr[0]);
    String_AllocLength(Dest, WordLength);
    memcpy(Dest -> Data, Curr, WordLength);
    Dest -> WordNum = 1;
}

void String_SetWord(String* This, int Index, String* Data)
{
    RAssert(Data -> WordNum > 0);
    
    char* Curr = This -> Data;
    int RIndex = 0;
    
    for(int i = 0; i < Index && * Curr; ++ i, ++ RIndex)
        Curr = next_char_utf8(Curr);
    
    RAssert(* Curr);
    
    int WordLengthT = GetWordLength_Utf8(Curr[0]);
    int WordLengthD = GetWordLength_Utf8(Data -> Data[0]);
    
    int DeltaLength = WordLengthD - WordLengthT;
    
    if(WordLengthD - WordLengthT)
    {
        int TLen = String_GetLength(This);
        int CpLen = TLen - RIndex - WordLengthT;
        char Buffer[CpLen];
        
        // Cp to buffer
        memcpy(Buffer, Curr + WordLengthT, CpLen);
        String_AllocLength(This, TLen + DeltaLength);
        Curr = This -> Data + RIndex;
        memcpy(Curr, Data -> Data, WordLengthD);
        memcpy(Curr + WordLengthD, Buffer, CpLen);
        
        // Update length.
        This -> Data_Index = TLen + DeltaLength - 1;
    }
    else
        memcpy(Curr, Data -> Data, WordLengthT);
}

void String_Join(String* This, String* Sorc)
{
    Array_Resize(char, This -> Data, 
                 This -> Data_Index + Sorc -> Data_Index + 2 + 
                 This -> Data_Index / 10);
    memcpy(This -> Data + This -> Data_Index + 1, Sorc -> Data,
           Sorc -> Data_Index + 1);
    This -> Data_Index += Sorc -> Data_Index + 1;
    This -> WordNum += Sorc -> WordNum;
}

void String_JoinChars(String* This, char* Sorc)
{
    int Len = strlen(Sorc);
    Array_Resize(char, This -> Data,
                 This -> Data_Index + Len + 1 + This -> Data_Index / 10);
    memcpy(This -> Data + This -> Data_Index + 1, Sorc, Len);
    This -> Data_Index += Len;
    This -> WordNum += strnum_utf8(Sorc);
}

int String_Equal(String* This, String* Sorc)
{
    return (This -> Data_Index == Sorc -> Data_Index) &&
           (! strncmp(This -> Data, Sorc -> Data, This -> Data_Index + 1));
}

int String_EqualN(String* This, String* Sorc, int w)
{
    return (w == Sorc -> WordNum) && 
    (! strwcmp_utf8(This -> Data, Sorc -> Data, w));
}

int String_EqualChars(String* This, const char* Sorc)
{
    if(strlen(Sorc) != This -> Data_Index + 1)
        return 0;
    return (! strncmp(This -> Data, Sorc, This -> Data_Index + 1));
}

int String_EqualNChars(String* This, const char* Sorc, int w)
{
    return ! strwcmp_utf8(This -> Data, Sorc, w);
}

int String_InStrFrom(String* This, String* Part, int From)
{
    int CurrWord = From;
    
    Array_Push(char, This -> Data, 0);
    -- This -> Data_Index;
    
    Array_Push(char, Part -> Data, 0);
    -- Part -> Data_Index;
    
    char* Curr = This -> Data;
    
    int ThisLen = String_GetLength(This);
    int PartLen = String_GetLength(Part);
    
    for(int i = 0; i < From; ++ i)
        Curr = next_char_safe_utf8(Curr, Curr + ThisLen);
    
    do
    {
        for(int j = 0; j < PartLen; ++ j)
        {
            if(Part -> Data[j] != Curr[j])
                goto cont;
        }
        return CurrWord;
        cont:;
        ++ CurrWord;
    } while((Curr = next_char_safe_utf8(Curr, Curr + ThisLen)));
    
    return -1;
}

int String_InStr(String* This, String* Part)
{
    return String_InStrFrom(This, Part, 0);
}

int String_InStrRevFrom(String* This, String* Part, int From)
{
    int CurrWord = From;
    
    Array_Push(char, This -> Data, 0);
    -- This -> Data_Index;
    
    Array_Push(char, Part -> Data, 0);
    -- Part -> Data_Index;
    
    char* Curr = This -> Data;
    
    int ThisLen = String_GetLength(This);
    int PartLen = String_GetLength(Part);
    
    for(int i = 0; i < From; ++ i)
        Curr = next_char_safe_utf8(Curr, Curr + ThisLen);
    
    do
    {
        for(int j = 0; j < PartLen; ++ j)
        {
            if(Part -> Data[j] != Curr[j])
                goto cont;
        }
        return CurrWord;
        cont:;
        -- CurrWord;
    } while((Curr = prev_char_safe_utf8(Curr, This -> Data)));
    
    return -1;
}

int String_InStrRev(String* This, String* Part)
{
    return String_InStrRevFrom(This, Part, String_GetWordNum(This));
}

void Mid(String* Dest, String* Sorc, int From, int Count)
{
    char* Begin = Sorc -> Data;
    char* End = NULL;
    
    for(int i = 0; i < From && Begin; ++ i)
        Begin = next_char_safe_utf8(Begin, 
                                    Sorc -> Data + Sorc -> Data_Index + 1);
        RAssert(Begin);
    End = Begin;
    
    for(int i = 0; i < Count && End; ++ i)
        End = next_char_safe_utf8(End, 
                                  Sorc -> Data + Sorc -> Data_Index + 1);
        RAssert(End);
    
    int MidLen = End - Begin;
    String_AllocLength(Dest, MidLen);
    memcpy(Dest -> Data, Begin, MidLen);
    
    // Update length.
    Dest -> Data_Index = MidLen - 1;
    Dest -> WordNum = Count;
}

void MidFrom(String* Dest, String* Sorc, int From)
{
    char* Begin = Sorc -> Data;
    char* End = Sorc -> Data + Sorc -> Data_Index + 1;
    
    for(int i = 0; i < From; ++ i)
        Begin = next_char_safe_utf8(Begin, End);
        RAssert(Begin);
    
    int MidLen = End - Begin;
    String_AllocLength(Dest, MidLen);
    
    memcpy(Dest -> Data, Begin, MidLen);
    Dest -> Data_Index = MidLen - 1;
    Dest -> WordNum = Sorc -> WordNum - From;
}

void Right(String* Dest, String* Sorc, int Count)
{
    char* End = Sorc -> Data + Sorc -> Data_Index + 1;
    char* Begin = End;
    for(int i = 0; i < Count; ++ i)
        Begin = prev_char_safe_utf8(Begin, Sorc -> Data);
        RAssert(Begin);
    
    int Len = End - Begin;
    String_AllocLength(Dest, Len);
    
    memcpy(Dest -> Data, Begin, Len);
    Dest -> Data_Index = Len - 1;
    Dest -> WordNum = Count;
}

void Left(String* Dest, String* Sorc, int Count)
{
    char* Begin = Sorc -> Data;
    char* End = Begin;
    for(int i = 0; i < Count; ++ i)
        End = next_char_safe_utf8(End, 
                                  Sorc -> Data + Sorc -> Data_Index + 1);
        RAssert(End);
    
    int Len = End - Begin;
    String_AllocLength(Dest, Len);
    
    memcpy(Dest -> Data, Begin, Len);
    Dest -> Data_Index = Len - 1;
    Dest -> WordNum = Count;
}

void UpperCase(String* Dest, String* Sorc)
{
    String_Copy(Dest, Sorc);
    char* Curr = String_GetChars(Dest);
    
    while(Curr && *Curr)
    {
        if(GetWordLength_Utf8(Curr[0]) == 1 && 
            (Curr[0] >= 'a' && Curr[0] <= 'z'))
            Curr[0] += ('A' - 'a');
        Curr = next_char_safe_utf8(Curr, 
                                   Dest -> Data + Dest -> Data_Index + 1);
    }
}

void LowerCase(String* Dest, String* Sorc)
{
    String_Copy(Dest, Sorc);
    char* Curr = String_GetChars(Dest);
    
    while(Curr && *Curr)
    {
        if(GetWordLength_Utf8(Curr[0]) == 1 && 
            (Curr[0] >= 'A' && Curr[0] <= 'Z'))
            Curr[0] -= ('A' - 'a');
        Curr = next_char_safe_utf8(Curr, 
                                   Dest -> Data + Dest -> Data_Index + 1);
    }
}

void Trim(String* Dest, String* Sorc)
{
    char* Begin = Sorc -> Data;
    char* End = Sorc -> Data + Sorc -> Data_Index;
    int LSkip, RSkip;
    
    for(LSkip = 0; Begin && (Begin[0] == ' ' || Begin[0] == '\t'); ++ LSkip)
        Begin = next_char_safe_utf8(Begin, End);
    if(! Begin)
        return;
    
    for(RSkip = 0; End && (End[0] == ' ' || End[0] == '\t'); ++ RSkip)
        End = prev_char_safe_utf8(End, Begin);
    if(! End)
        return;
    
    End = next_char_utf8(End);
    
    String_AllocLength(Dest, End - Begin);
    
    Dest -> WordNum = Sorc -> WordNum - LSkip - RSkip;
    Dest -> Data_Index = End - Begin - 1;
    
    memcpy(Dest -> Data, Begin, End - Begin);
}

void LTrim(String* Dest, String* Sorc)
{
    char* Begin = Sorc -> Data;
    char* End = Sorc -> Data + Sorc -> Data_Index;
    int LSkip;
    
    for(LSkip = 0; Begin && (Begin[0] == ' ' || Begin[0] == '\t'); ++ LSkip)
        Begin = next_char_safe_utf8(Begin, End);
    if(! Begin)
        return;
    
    End = next_char_utf8(End);
    
    String_AllocLength(Dest, End - Begin);
    
    Dest -> WordNum = Sorc -> WordNum - LSkip;
    Dest -> Data_Index = End - Begin - 1;
    
    memcpy(Dest -> Data, Begin, End - Begin);
}

void RTrim(String* Dest, String* Sorc)
{
    char* Begin = Sorc -> Data;
    char* End = Sorc -> Data + Sorc -> Data_Index;
    int RSkip;
    
    for(RSkip = 0; End && (End[0] == ' ' || End[0] == '\t'); ++ RSkip)
        End = prev_char_safe_utf8(End, Begin);
    if(! End)
        return;
    
    End = next_char_utf8(End);
    
    String_AllocLength(Dest, End - Begin);
    
    Dest -> WordNum = Sorc -> WordNum - RSkip;
    Dest -> Data_Index = End - Begin - 1;
    
    memcpy(Dest -> Data, Begin, End - Begin);
}

int _Wildcard_Match_Core(const char *WC, const char *Str)
{  
    if (*WC == '\0')  
        return *Str == '\0';  
    if (*WC == '?')  
        return _Wildcard_Match_Core(WC = next_char_utf8(WC), 
                                    Str = next_char_utf8(Str));  
    else if (*WC == '*')
    {  
        for (WC = next_char_utf8(WC); *Str; Str = next_char_utf8(Str)) 
            if (_Wildcard_Match_Core(WC, Str)) return 1;  
        return *WC == '\0';  
    }
    else  
        return *WC == *Str && _Wildcard_Match_Core(WC = next_char_utf8(WC),
                                                   Str = next_char_utf8(Str));
    
    return -1;
}  

int Wildcard_Match(String* Sorc, String* Wildcard)
{
    const char* Str = String_GetChars(Sorc);
    const char* WC = String_GetChars(Wildcard);
    return _Wildcard_Match_Core(WC, Str);
}
