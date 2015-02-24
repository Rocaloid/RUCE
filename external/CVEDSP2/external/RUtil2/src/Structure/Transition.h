#ifndef RUTIL2_TRANSITION_H
#define RUTIL2_TRANSITION_H

#include "../Core/OO.h"

#if 0
#include "_Transition.h"
#endif
#ifdef __RUtil2_Install
#define _RTAddress "RUtil2/Structure/_Transition.h"
#else
#define _RTAddress "Structure/_Transition.h"
#endif
#define _ClassName Transition_
#define _Attr 2

#define _T1 Float
#define _T2 Float
#include "../Core/RTemplate.h"

#define _T1 Float
#define _T2 Double
#include "../Core/RTemplate.h"

#define _T1 Double
#define _T2 Float
#include "../Core/RTemplate.h"

#define _T1 Double
#define _T2 Double
#include "../Core/RTemplate.h"

#endif //RUTIL2_TRANSITION_H

