#define _Wave _C(CDSP2_Wave, _, _T1)
#define _IWave _C(CDSP2_IWave, _, _T1)
#define _PMatch _C(PMatch_, _T1, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _PMatch AmplList;
    
    //Private
    void* Wave;
    void* OutWave;
    int LastPosition;
    int InitPosition;
};

RTMethod(void, CDSP2_GainIterfector, SetWave, void* Sorc);
RTMethod(void, CDSP2_GainIterfector, SetOutWave, void* Sorc);
RTMethod(void, CDSP2_GainIterfector, SetPosition, int Position);

RTMethod(void, CDSP2_GainIterfector, IterNextTo, int Position);
RTMethod(void, CDSP2_GainIterfector, IterPrevTo, int Position);
RTMethod(void, CDSP2_GainIterfector, PrevTo, int Position);

RTMethod(void, CDSP2_GainIterfector, Add, _T1 Ampl, int Position);

#undef  _Wave
#undef  _IWave
#undef  _PMatch

