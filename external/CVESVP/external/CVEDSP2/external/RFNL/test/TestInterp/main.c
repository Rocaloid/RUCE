#include <RUtil2.h>
#include <math.h>
#include "Interp/Interp.h"

void RFNL_Use();
int main()
{
    int i;
    RFNL_Use();
    Float src[1024];
    Float dst[1024];
    Float dst2[5000];
    for(i = 0; i < 1024; i ++)
    {
        src[i] = RFNL_PSin_Gnrc_Float(i * 0.05f) * i
               + fabs(RFNL_PTan_Gnrc_Float(i)) > 0.5 ?
                 0.5 : fabs(RFNL_PTan_Gnrc_Float(i));
    }
    PMatch_Float_Float Dest;
    RNew(PMatch_Float_Float, & Dest);
    RFNL_GenPeak_Gnrc_Float_Float(& Dest, src, 50, 1024);
    //PMatch_Float_Float_Print(& Dest);
    RFNL_MatchInterp_Cosine_Gnrc_Float_Float(src, & Dest, TopOf(Dest.Y), 1024);
    
    Double SorcMatch[6] = {0, 200, 400, 700, 900, 1023};
    Double DestMatch[6] = {0, 100, 200, 800, 950, 1023};
    //for(i = 0; i < 100000; i ++)
    RFNL_MapStretch_Linear_Gnrc_Double_Float(
        dst, src, DestMatch, SorcMatch, 6, 1024);
    RFNL_Resample_Non_Gnrc_Float(dst2, dst, 5000, 1024);
    
    for(i = 0; i < 5000; i ++)
        printf("%4d %9f\n", i, dst2[i]);
    RDelete(& Dest);
    return 0;
}

