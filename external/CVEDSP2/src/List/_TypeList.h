#define _T2 _C(CDSP2_Spectrum, _, _T1)
#define _T3 Spectrum
#include "_SizeNumList.h"
#undef  _T2
#undef  _T3

#define _T2 _C(CDSP2_DataFrame, _, _T1)
#define _T3 DataFrame
#define _CDSP2_NOSIZE

#include "_SizeNumList.h"

RTMethod(void, CDSP2_List_DataFrame, FromWave, void* SorcWave,
    CDSP2_List_Int* SorcPulse);

#undef  _CDSP2_NOSIZE
#undef  _T2
#undef  _T3

