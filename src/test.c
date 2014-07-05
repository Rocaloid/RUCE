#include <memory.h>
#include <stdio.h>
#include <RUtil2.h>
#include "Pitchbend.h"

static char TestStr[] = "AA#2#AC#3#FACC#255#";

int main()
{
    String_FromChars(s, TestStr);
    int len = RUCE_Pitchbend_GetLength(& s);
    printf("%d\n", len);
    
    short* Dest = calloc(len * sizeof(short), 1);
    
    RUCE_Pitchbend_Decode(Dest, & s);
    
    for(int i = 0; i < len; ++i)
        printf("Dest[%d] = %hd\n", i, Dest[i]);
    
    free(Dest);
    String_Dtor(& s);
    return 0;
}