#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include "Core/RAlloc.h"
#include "Structure/String.h"
#include "Misc/Base64.h"
#include "Core/OO.h"

int TestNr = 0;

static char TestCStr1[] = "What a fucky day yesterday!";        //27 % 3 == 0.
static char TestCStr2[] = "Bad luck always around me!!!";       //28 % 3 == 1.
static char TestCStr3[] = "So is today. Do you think so?";      //29 % 3 == 2.
static char TestCStr4[] = "";                                   //Empty string.
static char TestCStr5[] = "I\nC\nA\nN\nF\nL\nY\n!";             // with '\n'.
static char TestCStr6[] = "All tests should be passed~!!";      //29 % 3 == 2.

static void TestBase64_StrAddChar(String *Str, int WarpLen, char ToAdd)
{
    RAssert(WarpLen > 0);
    int SLen = String_GetLength(Str);
    int Rem = SLen % WarpLen;
    int WNr = 0;
    char* Buffer = calloc(SLen + SLen / WarpLen + 1, 1);
    
    int i;
    
    for(i = 0; i < SLen - Rem; i += WarpLen)
    {
        memcpy(Buffer + i + WNr, Str -> Data + i, WarpLen);
        *(Buffer + i + WarpLen + WNr) = ToAdd;
        ++WNr;
    }
    
    memcpy(Buffer + i + WNr, Str -> Data + SLen - Rem, Rem);
    
    String_SetCharsN(Str, Buffer, SLen + SLen / WarpLen);
    free(Buffer);
}

static int TestBase64_SubStrTest(char *CStr)
{
    int Ret = 0;
    printf("Base64 Test, Nr = %d: \n", ++ TestNr);
    
    String o;
    String p;
    String_Ctor(& o);
    String_Ctor(& p);
    
    int RawSize = (int)strlen(CStr);
    
    printf("    Raw string: '%s', Size = %d.\n", CStr, RawSize);
    
    int EncSize = Base64_Encode(& o, (void*)CStr, RawSize);
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    
    srand((int)tv.tv_usec);
    TestBase64_StrAddChar(& o, 
                          (int)(10.0f * rand() / (RAND_MAX + 1.0f)) + 1.0f, 
                          '\n');
    
    gettimeofday(&tv, NULL);
    srand((int)tv.tv_usec);
    TestBase64_StrAddChar(& o, 
                          (int)(10.0f * rand() / (RAND_MAX + 1.0f)) + 1.0f, 
                          ' ');
    
    printf("    Base64 encoded:"
        "String"    "= '%s', "
        "Size"      "= %d, "
        "RetS"      "= %d. \n",
        String_GetChars(& o), 
        String_GetLength(& o), 
        EncSize);
    
    
    
    int OSize = Base64_DecodeSize(String_GetLength(& o));
    String_AllocLength(& p, OSize);
    p.Data_Index = Base64_Decode((void*)p.Data, & o) - 1;
    
    printf("    Base64 decoded:"
        "String"    "= '%s', "
        "Size"      "= %d, "
        "RetS"      "= %d, "
        "RetEst"    "= %d. \n",
        String_GetChars(& p), 
        String_GetLength(& p), 
        p.Data_Index + 1, 
        OSize);
    
    if((Ret = ! (String_EqualChars(& p, CStr))))
        printf("[Error] Test isn't passed! Nr = %d!\n", TestNr);

    String_Dtor(& p);
    String_Dtor(& o);
    return Ret;
}

int main()
{
    if(TestBase64_SubStrTest(TestCStr1)) return 1;
    if(TestBase64_SubStrTest(TestCStr2)) return 1;
    if(TestBase64_SubStrTest(TestCStr3)) return 1;
    if(TestBase64_SubStrTest(TestCStr4)) return 1;
    if(TestBase64_SubStrTest(TestCStr5)) return 1;
    if(TestBase64_SubStrTest(TestCStr6)) return 1;
    printf("\n");
    return 0;
}

char *BLI_str_find_prev_char_utf8(const char *str, const char *p)
{
    for (--p; p >= str; --p) {
        if ((*p & 0xc0) != 0x80) {
            return (char *)p;
        }
    }
    return NULL;
}