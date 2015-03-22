#define _Wave _C(CDSP2_Wave, _, _T1)
#define _List_Int CDSP2_List_Int
#define _List_Sinusoid _C(CSVP_List_Sinusoid, _, _T1)
#define _List_DataFrame _C(CDSP2_List_DataFrame, _, _T1)

RTFunc(void, CSVP_SinusoidalFromSinuList, _Wave* Dest, _List_Int* PulseList,
    _List_Sinusoid* SinuList, _List_DataFrame* PhseList);
RTFunc(void, CSVP_NoiseTurbFromSinuList, _Wave* Dest, _Wave* Sorc,
    _List_Int* PulseList, _List_Sinusoid* SinuList, _List_DataFrame* PhseList);
RTFunc(void, CSVP_NoiseTurbFromWave, _Wave* Dest, _Wave* Sorc, _Wave* Sinu);

#undef  _Wave
#undef  _List_Int
#undef  _List_Sinusoid
#undef  _List_DataFrame

