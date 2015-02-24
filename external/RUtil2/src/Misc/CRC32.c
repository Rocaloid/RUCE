#include <unistd.h>
#include "CRC32.h"
#ifdef __BIG_ENDIAN__
    #define A(x) ((x) >> 24)
    #define B(x) (((x) >> 16) & 0xFF)
    #define C(x) (((x) >> 8) & 0xFF)
    #define D(x) ((x) & 0xFF)

    #define S8(x) ((x) << 8)
    #define S32(x) ((x) << 32)

#else
    #define A(x) ((x) & 0xFF)
    #define B(x) (((x) >> 8) & 0xFF)
    #define C(x) (((x) >> 16) & 0xFF)
    #define D(x) ((x) >> 24)

    #define S8(x) ((x) >> 8)
    #define S32(x) ((x) >> 32)
#endif
#include "../Core/OO.h"

static uint32_t CRC32_TABLE[8][256];

void __attribute__ ((constructor(1000))) __Create_CRC32_Table()
{
    static const uint32_t poly32 = UINT32_C(0xEDB88320);

    for (size_t s = 0; s < 8; ++ s) {
        for (size_t b = 0; b < 256; ++ b) {
            uint32_t r = s == 0 ? b : CRC32_TABLE[s - 1][b];

            for (size_t i = 0; i < 8; ++ i) {
                if (r & 1)
                    r = (r >> 1) ^ poly32;
                else
                    r >>= 1;
            }

            CRC32_TABLE[s][b] = r;
        }
    }

    #ifdef __BIG_ENDIAN__
        for (size_t s = 0; s < 8; ++ s)
        {
            for (size_t b = 0; b < 256; ++ b)
                Endian_Switch_Uint32(& (CRC32_TABLE[s][b]));
        }
    #endif
}

UInt CRC32Sum(const void* Buffer_P, size_t Size, UInt CRC)
{
    const UChar* Buffer = Buffer_P;
    
    CRC = ~CRC;

#ifdef __BIG_ENDIAN__
    CRC = Endian_Switch_Uint32(CRC);
#endif

    if (Size > 8) {
        // Fix the alignment, if needed. The if statement above
        // ensures that this won't read past the end of Buffer[].
        while ((uintptr_t)(Buffer) & 7) {
            CRC = CRC32_TABLE[0][*Buffer ++ ^ A(CRC)] ^ S8(CRC);
            --Size;
        }

        // Calculate the position where to stop.
        const UChar* const limit = Buffer + (Size & ~(size_t)(7));

        // Calculate how many bytes must be calculated separately
        // before returning the result.
        Size &= (size_t)(7);

        // Calculate the CRC32 using the slice-by-eight algorithm.
        while (Buffer < limit) {
            CRC ^=*(const UInt*)(Buffer);
            Buffer += 4;

            CRC = CRC32_TABLE[7][A(CRC)]
                ^ CRC32_TABLE[6][B(CRC)]
                ^ CRC32_TABLE[5][C(CRC)]
                ^ CRC32_TABLE[4][D(CRC)];

            const UInt tmp =*(const UInt*)(Buffer);
            Buffer += 4;

            // At least with some compilers, it is critical for
            // performance, that the CRC variable is XORed
            // between the two table-lookup pairs.
            CRC = CRC32_TABLE[3][A(tmp)]
                ^ CRC32_TABLE[2][B(tmp)]
                ^ CRC
                ^ CRC32_TABLE[1][C(tmp)]
                ^ CRC32_TABLE[0][D(tmp)];
        }
    }

    while ((Size --) != 0)
        CRC = CRC32_TABLE[0][*(Buffer ++) ^ A(CRC)] ^ S8(CRC);

#ifdef __BIG_ENDIAN__
    CRC = Endian_Switch_UInt32(CRC);
#endif

    return ~CRC;
}
