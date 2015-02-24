#ifndef CDSP2_SETTING_H
#define CDSP2_SETTING_H

#include <RUtil2.h>

#define __CDSP2_DEF_PREFIX extern

#include "RFNLInterface/Vec.h"
#include "RFNLInterface/DFT.h"
#include "RFNLInterface/FWindow.h"
#include "RFNLInterface/Interp.h"

#undef  __CDSP2_DEF_PREFIX

typedef enum
{
    CDSP2_Arch_Gnrc = 0,
    CDSP2_Arch_SSE = 1,
    CDSP2_Arch_AVX = 2
} CDSP2_ArchType;

#define CDSP2_Debug_Check 0

extern CDSP2_ArchType CDSP2_Arch;

int CDSP2_SetArch(CDSP2_ArchType Sorc);

#endif

