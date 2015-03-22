#ifndef CSVP_LISTS_H
#define CSVP_LISTS_H

#include <CVEDSP2.h>
#include "../Structure/HNMFrame.h"

#if 0
#include "_TypeList.h"
#endif

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Misc/_TypeList.h"
#else
#define _RTAddress "Misc/_TypeList.h"
#endif

#define _ClassName CSVP_List_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

