#define _PMatch _C(PMatch_, _T1, _, _T1)

RTFunc(int, CSVP_VOTFromWave, void* Sorc, int LBound, int RBound);
RTFunc(int, CSVP_OnsetFromWave, void* Sorc, _T1 Threshold, int LBound,
    int RBound);
RTFunc(int, CSVP_VOTFromF0Match, _PMatch* Sorc, _T1 DeltaFreq, int Num,
    int Length);

#undef  _PMatch

