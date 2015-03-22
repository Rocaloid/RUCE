#ifndef CDSP2_LISTS_H
#define CDSP2_LISTS_H

#include "../Container/Spectrum.h"
#include "../Container/Wave.h"

#include "List_Int.h"
#include "../Container/DataFrame.h"

#if 0
#include "_TypeList.h"
#endif

//-----
#include <RUtil2.h>

#ifdef __CDSP2_Install
#define _RTAddress "CVEDSP2/List/_TypeList.h"
#else
#define _RTAddress "List/_TypeList.h"
#endif

#define _ClassName CDSP2_List_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

