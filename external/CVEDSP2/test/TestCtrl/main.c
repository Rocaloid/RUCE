#include "Interface/Setting.h"

int main()
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
    RFNL_FWindow_Gnrc_Float fwin;
    RFNL_FWindow_Gnrc_Float_Ctor(& fwin);
    CDSP2_FWindow_Float_SetPara(& fwin, 21, 1080, 50);
    CDSP2_FWindow_Float_SetFunc(& fwin, 
        RFNL_Blackman_Size_Gnrc_Float, RFNL_Blackman_Valu_Gnrc_Float);
    CDSP2_FWindow_Float_Initialize(& fwin);
    
    // int i;
    float Dest[1024];
    float Wind[1024];
    CDSP2_VSet_Float(Wind, 1, 1024);
    CDSP2_Blackman_Float(Wind, Wind, 1024);
    
    int Size = 1024;
    CDSP2_FWindow_Float_Generate(& fwin, Dest, Size);
    //for(i = 0; i < Size; i ++)
    //    printf("%d %f\n", i, Dest[i]/* - Wind[i]*/);
    
    RFNL_FWindow_Gnrc_Float_Dtor(& fwin);
    return 0;
}

