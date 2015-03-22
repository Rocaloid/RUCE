#define _Wave _C(CDSP2_Wave, _, _T1)

RTFunc(_T1, CDSP2_EnergyFromWave, void* Sorc, int LBound, int Size);
RTFunc(_T1, CDSP2_MeanEnergyFromWave, void* Sorc, int LBound, int Size);
RTFunc(_T1, CDSP2_MeanEnergyFromWaveDB, void* Sorc, int LBound, int Size);

RTFunc(_T1, CDSP2_EnergyCurveFromWaveDB, _Wave* Dest, _Wave* Sorc, int Size);

#undef  _Wave

