#include "STFT.h"
#include "Misc/MiscUtils.h"
#include <RUtil2.h>

#if 0
#include "Iterator/_STFTIterlyzer.rc"
#include "Iterator/_STFTItersizer.rc"
#endif

//-----

#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_STFTIterlyzer.rc"
#else
#define _RTAddress "Iterator/_STFTIterlyzer.rc"
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
#define _RTAddress "CVESVP/Iterator/_STFTItersizer.rc"
#else
#define _RTAddress "Iterator/_STFTItersizer.rc"
#endif

#define _ClassName CSVP_STFTItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

