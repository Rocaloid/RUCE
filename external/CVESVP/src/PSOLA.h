#ifndef CSVP_PSOLA_H
#define CSVP_PSOLA_H

#include "Misc/Lists.h"
#include "STFT.h"
#include <CVEDSP2.h>
#include <RUtil2.h>

#if 0
#include "Iterator/_PSOLAIterlyzer.h"
#include "Iterator/_PSOLAItersizer.h"
#endif

#define CSVP_PSOLAIterlyzer_LPFSTFTSize 512
#define CSVP_PSOLAIterlyzer_LPFHopSize 256

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_PSOLAIterlyzer.h"
#else
#define _RTAddress "Iterator/_PSOLAIterlyzer.h"
#endif

#define _ClassName CSVP_PSOLAIterlyzer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----

#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_PSOLAItersizer.h"
#else
#define _RTAddress "Iterator/_PSOLAItersizer.h"
#endif

#define _ClassName CSVP_PSOLAItersizer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

