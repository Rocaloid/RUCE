#include "Setting.h"
#include <RFNL.h>

#define __CDSP2_DEF_PREFIX

#include "RFNLInterface/Vec.h"
#include "RFNLInterface/DFT.h"
#include "RFNLInterface/FWindow.h"
#include "RFNLInterface/Interp.h"

#undef  __CDSP2_DEF_PREFIX

CDSP2_ArchType CDSP2_Arch;

void __attribute__ ((constructor)) _CDSP2_LoadArch()
{
    CDSP2_SetArch(CDSP2_Arch_Gnrc);
}

int CDSP2_SetArch(CDSP2_ArchType Sorc)
{
    CDSP2_Arch = Sorc;
    switch(Sorc)
    {
        case CDSP2_Arch_SSE:
            break;
        case CDSP2_Arch_AVX:
            break;
        default:
            #undef  __Arch
            #define __Arch Gnrc
            #include "RFNLInterface/_DFT_SetArch.h"
            #include "RFNLInterface/_Vec_SetArch.h"
            #include "RFNLInterface/_FWindow_SetArch.h"
            #include "RFNLInterface/_Interp_SetArch.h"
            break;
    }
    return 1;
}

