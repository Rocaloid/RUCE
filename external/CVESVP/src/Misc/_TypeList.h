#define _T2 _C(CSVP_Sinusoid, _, _T1)
#define _T3 Sinusoid
#define _CDSP2_NOSIZE
#include <CVEDSP2/List/_SizeNumList.h>

#define _PMatch _C(PMatch_, _T1, _, _T1)
RTMethod(void, CSVP_List_Sinusoid, HToPMatch, _PMatch* Dest,
    CDSP2_List_Int* PulseList, int Index);
#undef  _PMatch

#undef  _CDSP2_NOSIZE
#undef  _T2
#undef  _T3

#define _T2 _C(CSVP_HNMFrame, _, _T1)
#define _T3 HNMFrame
#define _CDSP2_NOSIZE
#include <CVEDSP2/List/_SizeNumList.h>

#define _List_Sinusoid _C(CSVP_List_Sinusoid, _, _T1)
#define _PMatch _C(PMatch_, _T1, _, _T1)

RTMethod(void, CSVP_List_HNMFrame, ToSinuList, _List_Sinusoid* Dest);
RTMethod(void, CSVP_List_HNMFrame, HToPMatch, _PMatch* Dest,
    CDSP2_List_Int* PulseList, int Index);

#undef  _PMatch
#undef  _List_Sinusoid

#undef  _CDSP2_NOSIZE
#undef  _T2
#undef  _T3

#define _T2 _C(CSVP_HNMContour, _, _T1)
#define _T3 HNMContour
#include <CVEDSP2/List/_SizeNumList.h>
#undef  _T2
#undef  _T3

