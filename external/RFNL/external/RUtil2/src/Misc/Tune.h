#ifndef RUTIL2_TUNE_H
#define RUTIL2_TUNE_H

#include "../Structure/String.h"
#include "../Core/OO.h"

enum Tune_PitchNameMode
{
    Tune_AlwaysRising = 0,
    Tune_AlwaysFalling,
    Tune_Mixing,
    __PADDING__ = INT_MAX
};

void Tune_SetPitchNameMode(enum Tune_PitchNameMode m);

#if 0
#include "_Tune.h"
#endif
#ifdef __RUtil2_Install
#define _RTAddress "RUtil2/Misc/_Tune.h"
#else
#define _RTAddress "Misc/_Tune.h"
#endif

#define _ClassName
#define _Attr 1

#define _T1 Float
#include "../Core/RTemplate.h"

#define _T1 Double
#include "../Core/RTemplate.h"

#define _T1 Int
#include "../Core/RTemplate.h"

#endif //RUTIL2_TUNE_H
