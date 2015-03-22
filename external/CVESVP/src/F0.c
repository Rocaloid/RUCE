#include "F0.h"
#include <RUtil2.h>

#if 0
#include "_F0.rc"
#include "_YIN.rc"
#include "Iterator/_F0Iterlyzer.rc"
#endif

#define M_SPECSTEP 1
#define M_PEAK 2
#define M_YIN 3

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/_F0.rc"
#else
#define _RTAddress "_F0.rc"
#endif

#define _ClassName _
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/_YIN.rc"
#else
#define _RTAddress "_YIN.rc"
#endif

#define _ClassName _
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_F0Iterlyzer.rc"
#else
#define _RTAddress "Iterator/_F0Iterlyzer.rc"
#endif

#define _ClassName CSVP_F0Iterlyzer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

