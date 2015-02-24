//Generic DFT implementation

#include "DFT.h"
#include "../Vec/Vec.h"
#include <math.h>
#include <RUtil2.h>

#if 0
#include "_DFT_Gnrc.rc"
#endif
#define _RTAddress "DFT/_DFT_Gnrc.rc"

#define _ClassName
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

