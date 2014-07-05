#include <stdlib.h>
#include <RUtil2.h>

int RUCE_Pitchbend_GetLength(String* Sorc)
{
    int Len = 0;
    
    char* InputData = Sorc -> Data;
    int SorcLen = String_GetLength(Sorc);
    
    char RepeatBuffer[16];
    int BufferCursor = 0;
    
    for(int i = 0; i < SorcLen; ++i, ++BufferCursor)
    {
        if(InputData[i] == '#')
        {
            Len += BufferCursor / 2;
            BufferCursor = 0;
            
            while(InputData[++i] != '#')
                RepeatBuffer[BufferCursor ++] = InputData[i];
            RepeatBuffer[BufferCursor] = '\0';
            Len += atoi(RepeatBuffer);
            
            BufferCursor = 0;
        }
    }
    
    Len += BufferCursor / 2;
    
    return Len;
}

void RUCE_Pitchbend_Decode(short* Dest, String* Sorc)
{
    int Len = 0;
    
    char* InputData = Sorc -> Data;
    int SorcLen = String_GetLength(Sorc);
    
    char RepeatBuffer[16];
    int BufferCursor = 0;
    
    for(int i = 0; i < SorcLen; ++i)
    {
        if(InputData[i] == '#')
        {
            BufferCursor = 0;
            
            while(InputData[++i] != '#')
                RepeatBuffer[BufferCursor ++] = InputData[i];
            RepeatBuffer[BufferCursor] = '\0';
            
            int RepeatNr = atoi(RepeatBuffer);
            short RepeatData = Dest[Len - 1];
            for(int j = 0; j < RepeatNr; ++j)
                Dest[Len ++] = RepeatData;
            
            continue;
        }
        Dest[Len ++] = Base64_Decode_Int12(& (InputData[i ++]));
    }
}