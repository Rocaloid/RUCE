#ifndef RUCE_SESSIONCONF_H
#define RUCE_SESSIONCONF_H

#include <RUtil2.h>
#include "Common.h"

RClass(RUCE_SessionConfig)
{
    RInherit(RObject);
    
    Real* Window;
    int WinSize;
    Real HmncUpperBound;
};

#endif

