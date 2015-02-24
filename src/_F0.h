#define _Spectrum _C(CDSP2_Spectrum, _, _T1)
#define _PMatch _C(PMatch_, _T1, _, _T1)

RTFunc(int, CSVP_F0FromSpectrumB_SpecStep, _Spectrum* Sorc, _T1 MinFreq,
    _T1 MaxFreq);
RTFunc(int, CSVP_F0FromSpectrumB_Peak, _Spectrum* Sorc, _T1 MinFreq,
    _T1 MaxFreq);

RTFunc(_T1, CSVP_F0FromSuccSpectrumAndBin, _Spectrum* Sorc1, _Spectrum* Sorc2,
    int HopSize, int Bin);
RTFunc(_T1, CSVP_F0FromSuccSpectrum_SpecStep, _Spectrum* Sorc1,
    _Spectrum* Sorc2, int HopSize, _T1 MinFreq, _T1 MaxFreq);
RTFunc(_T1, CSVP_F0FromSuccSpectrum_Peak, _Spectrum* Sorc1, _Spectrum* Sorc2,
    int HopSize, _T1 MinFreq, _T1 MaxFreq);

RTFunc(_T1, CSVP_F0FromWave_SpecStep, void* Sorc, int Position,
    int MinFreq, int MaxFreq);
RTFunc(_T1, CSVP_F0FromWave_Peak, void* Sorc, int Position,
    int MinFreq, int MaxFreq);
RTFunc(_T1, CSVP_F0FromWave_YIN, void* Sorc, int Position, _T1 MinFreq, int W,
    _T1 Threshold);

RTFunc(_T1, CSVP_SecureF0FromWave, void* Sorc, int Center, 
    int LPosition, int RPosition, int MinFreq, int MaxFreq);

RTFunc(void, CSVP_F0PostProcess, _PMatch* Dest, int AnaSize, _T1 Variance,
    _T1 LFreq, _T1 HFreq);

#undef  _Spectrum
#undef  _PMatch

