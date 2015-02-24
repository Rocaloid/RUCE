#ifndef CSVP_HNM_H
#define CSVP_HNM_H

#include "Misc/Lists.h"
#include "STFT.h"
#include <CVEDSP2.h>
#include <RUtil2.h>

#define CSVP_SinusoidalPreserveSize 1024
#define CSVP_PhaseFlag_Lock    0
#define CSVP_PhaseFlag_Control 1
#define CSVP_PhaseFlag_Regen   2

#if 0
#include "Iterator/_SinusoidalBase.h"
#include "Iterator/_SinuGenericIterlyzer.h"
#include "Iterator/_SinusoidItersizer.h"
#include "Iterator/_HNMItersizer.h"
#endif

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_SinusoidalBase.h"
#else
#define _RTAddress "Iterator/_SinusoidalBase.h"
#endif

#define _ClassName CSVP_SinusoidalBase_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_SinuGenericIterlyzer.h"
#else
#define _RTAddress "Iterator/_SinuGenericIterlyzer.h"
#endif

#define _ClassName CSVP_SinusoidIterlyzer_
#define _Attr 1

#define _T4 SinuList
#define _T3 Sinusoid
#define _T2 CSVP_SinusoidIterlyzer
#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T4 SinuList
#define _T3 Sinusoid
#define _T2 CSVP_SinusoidIterlyzer
#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_SinusoidItersizer.h"
#else
#define _RTAddress "Iterator/_SinusoidItersizer.h"
#endif

#define _ClassName CSVP_SinusoidItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_SinuGenericIterlyzer.h"
#else
#define _RTAddress "Iterator/_SinuGenericIterlyzer.h"
#endif

#define _ClassName CSVP_HNMIterlyzer_
#define _Attr 1

#define _T4 HNMList
#define _T3 HNMFrame
#define _T2 CSVP_HNMIterlyzer
#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T4 HNMList
#define _T3 HNMFrame
#define _T2 CSVP_HNMIterlyzer
#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_HNMItersizer.h"
#else
#define _RTAddress "Iterator/_HNMItersizer.h"
#endif

#define _ClassName CSVP_HNMItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

