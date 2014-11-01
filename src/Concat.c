#include "Concat.h"
#include "Verbose.h"

// Return value
// >0: Length of fade out region after Align;
//  0: Failed
int RUCE_Concat_FadeOut(Wave* Dest, int Align, int Length)
{
    Verbose(3, "(function entrance) Size=%d, Align=%d, Length=%d\n",
        Dest -> Size, Align, Length);
    if(Dest -> Size - Align < Length)
        Length = Dest -> Size - Align;
    
    int i;
    for(i = 0; i < Length; i ++)
        Dest -> Data[i + Align] *= 1.0 - (Real)i / Length;
    for(i += Align; i < Dest -> Size; i ++)
        Dest -> Data[i] = 0;
    
    return Length;
}

// Return value
// >0: Length of fade in region after Align;
//  0: Failed
int RUCE_Concat_FadeIn(Wave* Dest, int Align, int Length)
{
    Verbose(3, "(function entrance) Align=%d, Length=%d\n", Align, Length);
    if(Align + Length > Dest -> Size)
        Length = Dest -> Size - Align;
    
    int i;
    for(i = 0; i < Align; i ++)
        Dest -> Data[i] = 0;
    for(i = 0; i < Length; i ++)
        Dest -> Data[Align + i] *= (Real)i / Length;
    
    return Length;
}

