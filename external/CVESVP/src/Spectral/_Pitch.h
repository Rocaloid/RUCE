#define _HNMFrame _C(CSVP_HNMFrame, _, _T1)
#define _HNMContour _C(CSVP_HNMContour, _, _T1)

RTFunc(void, CSVP_PitchShiftHNMContour, _HNMContour* Dest,
    CSVP_PitchModel* Sorc, _T1 F0, int SampleRate);

RTFunc(void, CSVP_PitchAdjustHNMContour, _HNMContour* Dest,
    CSVP_PitchModel* Sorc, _T1 F0, int SampleRate);

RTFunc(void, CSVP_PitchConvertHNMFrame, _HNMContour* Dest, _HNMFrame* Sorc,
    CSVP_PitchModel* Model, _T1 F0, int UFreq, int SampleRate);

#undef  _HNMFrame
#undef  _HNMContour

