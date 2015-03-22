#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "Misc/CRC32.h" 
#include "Core/OO.h"
#include "Core/RAlloc.h"

static int SPTNr = 0;

static int CRC32_Str_Compare(const char* Sorc, UInt Correct)
{
    UInt CRC = CRC32Sum(Sorc, strlen(Sorc), 0);
    if(CRC != Correct)
    {
        fprintf(stderr, "[Error] CRC32 worked out a wrong result: %08x\n"
                        "Correct is %08x.\n", CRC, Correct);
        return 0;
    }
    return 1;
}

static float __CRC32_SpeedTest_Core__(UInt TestCount, UInt TestSize)
{
    if(TestSize % 4)
    {
        fprintf(stderr, "[Error] TestSize must be muiltipie of 4!\n");
        return -1.0f;
    }
    
    struct timeval bgn, end;
    
    char** Data = RAlign(8, sizeof(char**) * TestCount);
    for(int i = 0; i < TestCount; ++ i)
    {
        Data[i] = RAlign(8, TestSize);
        for(int j = 0; j < TestSize; ++ j)
        {
            gettimeofday(& bgn, NULL);
            srand(bgn.tv_usec);
            Data[i][j] = (int)rand();
        }
    }
    
    gettimeofday(& bgn, NULL);
    
    for(int i = 0; i < TestCount; ++ i)
        CRC32Sum(Data[i], TestSize, 0);
    
    gettimeofday(& end, NULL);
    
    for(int i = 0; i < TestCount; ++ i)
        free(Data[i]);
    free(Data);
    
    float t = ((float)(end.tv_usec - bgn.tv_usec)) / 1000000.0f;
    
    printf("    SpeedTest Nr = %d, Time = %f.\n", ++ SPTNr, t);
    
    return t;
}

static float CRC32_SpeedTest(UInt DataSize)
{
    UInt Half = DataSize / 2;
    
    float t = __CRC32_SpeedTest_Core__(Half, 1048576);
    t += __CRC32_SpeedTest_Core__(1, Half * 1048576);
    
    printf("    SpeedTest Total Time = %f.\n", t);
    
    return t;
}

int main()
{
    if(CRC32_Str_Compare("Hello I'm a cuuuuuute kangaroo.", 0xd36dd1d0) != 1)
        return 1;
    
    if(CRC32_Str_Compare("来吧袋鼠！", 0xddc486a2) != 1)
        return 1;
    
    CRC32_SpeedTest(2);   // 128MB; 64 * 1M + 1 * 64M;
    
    return 0;
}