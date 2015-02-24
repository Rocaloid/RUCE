#ifndef __RUtil2_Install
#define __RUtil2_Install
#endif //__RUtil2_Install

#include <RUtil2/Core/RAlloc.h>

#include <RUtil2/Structure/Array.h>
#include <RUtil2/Structure/String.h>
#include <RUtil2/Structure/PMatch.h>
#include <RUtil2/Structure/UtfString.h>

#include <RUtil2/Misc/Convert.h>
#include <RUtil2/Misc/Sort.h>
#include <RUtil2/Misc/Base64.h>
#include <RUtil2/Misc/Tune.h>
#include <RUtil2/Misc/CRC32.h>
#include <RUtil2/Misc/Endian_Switch.h>

#include <RUtil2/IO/File.h>
#include <RUtil2/IO/WaveFile.h>
#include <RUtil2/Core/OO.h>

//Default
#define PMatch PMatch_Double_Double
#define PMatch_Ctor PMatch_Double_Double_Ctor
#define PMatch_Dtor PMatch_Double_Double_Dtor
#define PMatch_AddPair PMatch_Double_Double_AddPair
#define PMatch_Clear PMatch_Double_Double_Clear
#define PMatch_Print PMatch_Double_Double_Print
#define PMatch_Query PMatch_Double_Double_Query

