#include "Base64.h"
#include <string.h>
#include <stdint.h>
#include <assert.h>

static UChar RB64_Encoding_Table[] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 
    'w', 'x', 'y', 'z', '0', '1', '2', '3', 
    '4', '5', '6', '7', '8', '9', '+', '/' 
};

static UChar RB64_Decoding_Table[] =
{
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //0   - 15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //16  - 31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, //32  - 47
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, //48  - 63
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //64  - 79
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, //80  - 95
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //96  - 111
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, //112 - 127
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //128 - 143
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //144 - 159
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //160 - 175
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //176 - 191
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //192 - 207
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //208 - 223
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //224 - 239
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  //240 - 255
};

int Base64_DecodeSize(int StrSize)
{
    // RDebugPrint(Base64, "Decodesize: This function is experimental and may "
    //     "yield wrong result.");
    return ((StrSize + 3) >> 2) * 3;
}

int Base64_EncodeSize(int DataSize)
{
    return ((DataSize + 2) / 3) << 2;
}

int Base64_Encode(String* Dest, void* Sorc, int Size)
{
    // RDebugPrint(Base64, "Encode: This function is experimental and may "
    //     "yield wrong result.");
    if((! Sorc) || (! Size)) return 0;
    
    int Ret = Base64_EncodeSize(Size);
    
    String_AllocLength(Dest, Ret);
    
    UChar* CSorc = (UChar*)Sorc;
    UChar* DData = (UChar*)Dest -> Data;
    int Remaining = Size % 3;
    UChar* FDEnd = CSorc + Size - Remaining;
    
    UChar* CurrCSrc = CSorc;
    UChar* CurrDest = DData;
    
    if(! DData) return - 1;
    
    while(CurrCSrc != FDEnd)
    {
        CurrDest[0] = RB64_Encoding_Table[CurrCSrc[0] >> 2];
        CurrDest[1] = RB64_Encoding_Table[((CurrCSrc[0] & 0x03) << 4) | 
                                          ((CurrCSrc[1] & 0xf0) >> 4)];
        CurrDest[2] = RB64_Encoding_Table[((CurrCSrc[1] & 0x0f) << 2) | 
                                          ((CurrCSrc[2] & 0xc0) >> 6)];
        CurrDest[3] = RB64_Encoding_Table[CurrCSrc[2] & 0x3f];
        CurrCSrc += 3;
        CurrDest += 4;
    }
    
    if(Remaining > 0)
    {
        CurrDest[0] = RB64_Encoding_Table[CurrCSrc[0] >> 2];
        CurrDest[1] = RB64_Encoding_Table[((CurrCSrc[0] & 0x03) << 4) | 
                                          ((CurrCSrc[1] & 0xf0) >> 4)];
        CurrDest[2] = (Remaining > 1 ? RB64_Encoding_Table
                                        [((CurrCSrc[1] & 0x0f) << 2) | 
                                          ((CurrCSrc[2] & 0xc0) >> 6)] : '=');
        CurrDest[3] = (Remaining > 2 ? 
                       RB64_Encoding_Table[CurrCSrc[2] & 0x3f] : '=');
    }
    
    Dest -> Data_Index = Ret - 1;

    return Ret;
}

int Base64_Decode(void* Dest, String* Sorc)
{
    // RDebugPrint(Base64, "Decode: This function is experimental and may "
    //     "yield wrong result.");
    if(! String_GetLength(Sorc)) return 0;
    assert(Dest);
    
    UChar* CDest = (UChar*)Dest;
    int Ret = 0;
    CDest[0] = 0;
    
    int RawLen = String_GetLength(Sorc);
    int SLen = 0;
    
    // Remove the '\n'.
    UChar* InputChars = calloc(RawLen, 1);
    for(int i = 0; i < RawLen; ++i)
    {
        if(Sorc -> Data[i] == '\n' || Sorc -> Data[i] == ' ')
            continue;
        InputChars[SLen ++] = Sorc -> Data[i];
    }
    
    // Check whether the input is valid.
    for(int i = 0; i < SLen; ++i)
    {
        if(! (InputChars[i] >= '0' || InputChars[i] <='9' ||
              InputChars[i] >= 'A' || InputChars[i] <='Z' ||
              InputChars[i] >= 'a' || InputChars[i] <='z'))
        {
            fprintf(stderr, "[Error] Base64_Decode: Invalid input string!");
            return -2;
        }
    }
    
    if(SLen < 4 || SLen % 4 != 0) return -1;
    
    // 0xFC -> 11111100
    // 0x03 -> 00000011
    // 0xF0 -> 11110000
    // 0x0F -> 00001111
    // 0xC0 -> 11000000
    UChar* Curr;
    UChar* SEnd = InputChars + SLen;
    for(Curr = InputChars; Curr < SEnd; Curr += 4)
    {
        *CDest ++ = ((RB64_Decoding_Table[Curr[0]] << 2) & 0xFC) | 
                    ((RB64_Decoding_Table[Curr[1]] >> 4) & 0x03);
        *CDest ++ = ((RB64_Decoding_Table[Curr[1]] << 4) & 0xF0) |
                    ((RB64_Decoding_Table[Curr[2]] >> 2) & 0x0F);
        *CDest ++ = ((RB64_Decoding_Table[Curr[2]] << 6) & 0xC0) |
                    (RB64_Decoding_Table[Curr[3]]);
        Ret += 3;
    }
    
    if(*(InputChars + SLen - 2) == '=')
    {
        *(CDest - 2) = '\0';
        Ret -= 2;
    }
    else if(*(InputChars + SLen - 1) == '=')
    {
        *(CDest - 1) = '\0';
        Ret -= 1;
    }
    
    free(InputChars);
    
    return Ret;
}

short Base64_Decode_Int12(char* Sorc)
{
    return (RB64_Decoding_Table[(UChar)Sorc[0]] << 6) + 
           (RB64_Decoding_Table[(UChar)Sorc[1]]);
}
