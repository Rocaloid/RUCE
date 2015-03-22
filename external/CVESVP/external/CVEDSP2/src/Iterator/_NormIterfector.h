#define _Wave _C(CDSP2_Wave, _, _T1)
#define _IWave _C(CDSP2_IWave, _, _T1)
#define _Gain _C(CDSP2_GainIterfector, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _T1 Gain;
    _T1 Intensity;
    int HopSize;
    
    //Private
    _Gain* Subsizer;
    void* ECurve;
    void* Wave;
    int LastPosition;
    int InitPosition;
};

RTMethod(void, CDSP2_NormIterfector, SetWave, void* Sorc);
RTMethod(void, CDSP2_NormIterfector, SetOutWave, void* Sorc);
RTMethod(void, CDSP2_NormIterfector, SetEnergyWave, void* Sorc);
RTMethod(void, CDSP2_NormIterfector, SetHopSize, int HopSize);
RTMethod(void, CDSP2_NormIterfector, SetGain, _T1 Gain);
RTMethod(void, CDSP2_NormIterfector, SetIntensity, _T1 Intensity);
RTMethod(void, CDSP2_NormIterfector, SetPosition, int Position);

RTMethod(void, CDSP2_NormIterfector, IterNextTo, int Position);
RTMethod(void, CDSP2_NormIterfector, IterPrevTo, int Position);
RTMethod(void, CDSP2_NormIterfector, PrevTo, int Position);

#undef  _Wave
#undef  _IWave
#undef  _Gain

