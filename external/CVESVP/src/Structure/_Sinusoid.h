#define _Spectrum _C(CDSP2_Spectrum, _, _T1)
#define _DataFrame _C(CDSP2_DataFrame, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);

    //Public
    int Size;
    Array_Define(_T1, Freq);
    Array_Define(_T1, Ampl);
};

RTMethod(void, CSVP_Sinusoid, CtorSize, int Size);
RTMethod(void, CSVP_Sinusoid, Resize, int Size);
RTMethod(void, CSVP_Sinusoid, From, _RTClassName* Sorc);
RTMethod(void, CSVP_Sinusoid, InterpFrom, _RTClassName* Sorc1,
    _RTClassName* Sorc2, _T1 Ratio);

RTMethod(void, CSVP_Sinusoid, Clear);

RTMethod(void, CSVP_Sinusoid, ToReal, _T1* Dest, int Size, int SampleRate);
RTMethod(void, CSVP_Sinusoid, ToSpectrum, _Spectrum* Dest);
RTMethod(void, CSVP_Sinusoid, FromSpectrum, _Spectrum* Sorc, _T1 F0);
RTMethod(void, CSVP_Sinusoid, FromSpectrumWithPhase, _DataFrame* Dest,
    _Spectrum* Sorc, _T1 F0);
RTMethod(void, CSVP_Sinusoid, SFromSpectrum, _Spectrum* Sorc,
    _RTClassName* Prev, _T1 F0);
RTMethod(void, CSVP_Sinusoid, SFromSpectrumWithPhase, _DataFrame* Dest,
    _Spectrum* Sorc, _RTClassName* Prev, _T1 F0);

RTMethod(void, CSVP_Sinusoid, GradientDescentWithPhase, _DataFrame* Phse,
    _T1* Sorc, int Size, int SampleRate, int Iteration, _T1 Alpha);
RTMethod(void, CSVP_Sinusoid, KillDeviatedHarmonics, _T1 Delta);

#undef  _Spectrum
#undef  _DataFrame

