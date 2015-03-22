//Discrete Fourier Transform Header

#ifndef RFNL_DFT_H
#define RFNL_DFT_H

#include "BitRev.h"
#include <RUtil2.h>

#if 0
#include "_DFT_Gnrc.h"
#endif

#ifdef __RFNL_Install
#define _RTAddress "RFNL/DFT/_DFT_Gnrc.h"
#else
#define _RTAddress "DFT/_DFT_Gnrc.h"
#endif

#define _ClassName
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

