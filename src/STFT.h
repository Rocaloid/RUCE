#ifndef CSVP_STFT_H
#define CSVP_STFT_H

#include "Misc/Lists.h"
#include <CVEDSP2.h>
#include <RUtil2.h>

#if 0
#include "Iterator/_STFTItersizer.h"
#include "Iterator/_STFTIterlyzer.h"
#endif

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_STFTIterlyzer.h"
#else
#define _RTAddress "Iterator/_STFTIterlyzer.h"
#endif

#define _ClassName CSVP_STFTIterlyzer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_STFTItersizer.h"
#else
#define _RTAddress "Iterator/_STFTItersizer.h"
#endif

#define _ClassName CSVP_STFTItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

