#define _HNMFrame _C(CSVP_HNMFrame, _, _T1)
#define _HNMContour _C(CSVP_HNMContour, _, _T1)

RTFunc(void, CSVP_ApplyDecay, _HNMContour* Dest, CSVP_SpectralDecay* Sorc,
    int SampleRate);

RTFunc(_T1 , CSVP_EnergyFromHNMFrame, _HNMFrame* Sorc);
RTFunc(_T1 , CSVP_EnergyFromHNMContour, _HNMContour* Sorc);

#undef  _HNMFrame
#undef  _HNMContour

