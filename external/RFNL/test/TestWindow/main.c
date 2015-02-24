#include <RUtil2.h>
#include <math.h>
#include "FWindow/FWindow.h"

void RFNL_Use();
int main()
{
    int i;
    RFNL_Use();
    RFNL_FWindow_Gnrc_Float fwin;
    RFNL_FWindow_Gnrc_Float_Ctor(& fwin);
    RFNL_FWindow_Gnrc_Float_SetPara(& fwin, 21, 1080, 50);
    RFNL_FWindow_Gnrc_Float_SetFunc(& fwin, 
        RFNL_Blackman_Size_Gnrc_Float, RFNL_Blackman_Valu_Gnrc_Float);
    RFNL_FWindow_Gnrc_Float_Initialize(& fwin);
    
    float Dest[1024];
    float Wind[1024];
    RFNL_VSet_Gnrc_Float(Wind, 1, 1024);
    RFNL_Blackman_Gnrc_Float(Wind, Wind, 1024);
    
    int Size = 1024;
    RFNL_FWindow_Gnrc_Float_Generate(& fwin, Dest, Size);
    for(i= 0; i < Size; i ++)
        printf("%d %f\n", i, Dest[i]/* - Wind[i]*/);
    
    RFNL_FWindow_Gnrc_Float_Dtor(& fwin);
    return 0;
}

