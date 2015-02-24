#define _Spectrum _C(CDSP2_Spectrum, _, _T1)
#define _List_Spectrum _C(CDSP2_List_Spectrum, _, _T1)

RTFunc(void, CSVP_SpecLowPass, _Spectrum* Dest, _T1 Freq);
RTFunc(void, CSVP_SpecHighPass, _Spectrum* Dest, _T1 Freq);
RTFunc(void, CSVP_SpecBandPass, _Spectrum* Dest, _T1 LFreq, _T1 HFreq);
RTFunc(void, CSVP_SpecBandStop, _Spectrum* Dest, _T1 LFreq, _T1 HFreq);

RTFunc(void, CSVP_SpecFilter, _Spectrum* Dest, _T1* Sorc);

RTFunc(void, CSVP_LSpecLowPass, _List_Spectrum* Dest, _T1 Freq);
RTFunc(void, CSVP_LSpecHighPass, _List_Spectrum* Dest, _T1 Freq);
RTFunc(void, CSVP_LSpecBandPass, _List_Spectrum* Dest, _T1 LFreq, _T1 HFreq);
RTFunc(void, CSVP_LSpecBandStop, _List_Spectrum* Dest, _T1 LFreq, _T1 HFreq);

RTFunc(void, CSVP_LSpecFilter, _List_Spectrum* Dest, _T1* Sorc);

#undef _Spectrum
#undef _List_Spectrum

