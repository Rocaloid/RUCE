//Architecture-independent 2nd Template DFT Interface

RTFunc(void, _C(RFNL_RFFT, _, _T2), _T1* DestReal, _T1* DestImag,
                                    _T1* SorcReal,
                                    int Power);
RTFunc(void, _C(RFNL_CFFT, _, _T2), _T1* DestReal, _T1* DestImag,
                                    _T1* SorcReal, _T1* SorcImag,
                                    int Power);
RTFunc(void, _C(RFNL_IRFFT, _, _T2), _T1* DestReal,
                                     _T1* SorcReal, _T1* SorcImag,
                                     int Power);
RTFunc(void, _C(RFNL_ICFFT, _, _T2), _T1* DestReal, _T1* DestImag,
                                     _T1* SorcReal, _T1* SorcImag,
                                     int Power);

