#define _DataFrame _C(CDSP2_DataFrame, _, _T1)
#define _Sinusoid _C(CSVP_Sinusoid, _, _T1)

RTFunc(void, CSVP_PhaseInterp, _DataFrame* Dest, _DataFrame* Sorc1,
    _DataFrame* Sorc2, _T1 Ratio);
RTFunc(void, CSVP_FPhaseInterp, _DataFrame* Dest, _DataFrame* Sorc1,
    _DataFrame* Sorc2, _Sinusoid* Sinu1, _Sinusoid* Sinu2, _T1 Time, _T1 Ratio);

RTFunc(void, CSVP_PhaseSync, _DataFrame* Dest, _T1* Freq, _T1 Phse0);
RTFunc(void, CSVP_PhaseSyncH, _DataFrame* Dest, _T1 Phse0);

RTFunc(void, CSVP_PhaseContract, _DataFrame* Dest, _T1 Ratio);

RTFunc(void, CSVP_PhaseShift, _DataFrame* Dest, _DataFrame* Sorc, _T1* Freq,
    int DeltaPos, int SampleRate);

RTFunc(void, CSVP_TPhaseShift, _DataFrame* Dest, _DataFrame* Sorc, _T1* Freq,
    _T1 Time);

RTFunc(void, CSVP_PhaseUnwrap, _DataFrame* Dest, _DataFrame* Sorc, _T1* SFreq,
    _T1* DFreq, int DeltaPos, int SampleRate, int Direction);

RTFunc(void, CSVP_PhaseWrap, _DataFrame* Dest);

#undef  _DataFrame
#undef  _Sinusoid

