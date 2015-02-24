#ifndef RUTIL2_PMATCH_H
#define RUTIL2_PMATCH_H

#include "Array.h"
#include "Transition.h"
#include "../Core/OO.h"

#if 0
#include "_PMatch.h"
#endif
#ifdef __RUtil2_Install
#define _RTAddress "RUtil2/Structure/_PMatch.h"
#else
#define _RTAddress "Structure/_PMatch.h"
#endif
#define _ClassName PMatch_
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

#endif //RUTIL2_PMATCH_H

