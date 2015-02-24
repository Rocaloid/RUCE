#define _Wave _C(CDSP2_Wave, _, _T1)
#define _List_Int CDSP2_List_Int
#define _List_GnrcFrame _C(CSVP_List_, _T3, _, _T1)
#define _List_DataFrame _C(CDSP2_List_DataFrame, _, _T1)
#define _STFTIterlyzer _C(CSVP_STFTIterlyzer, _, _T1)
#define _SinusoidalBase _C(CSVP_SinusoidalBase, _, _T1)
#define _PMatch _C(PMatch_, _T1, _, _T1)

RClass(_RTClassName)
{
    RInherit(_SinusoidalBase);
    
    //Public
    char GenPhase;
    int LeftBound;
    float DThreshold;  //Deviation threshold for harmonics
    float DFThreshold; //Deviation threshold for F0
    
    struct
    {
        _T1 Period;
        _T1 Alpha;
        int Iteration;
    } GDOption;
    
    _List_Int       PulseList;
    _List_GnrcFrame _T4;
    _List_DataFrame PhseList;
    
    //Private
    _T1 UFreq;
};

RTMethod(void, _T2, CtorSize, int Size);
RTMethod(void, _T2, Resize, int Size);
RTMethod(void, _T2, From, _RTClassName* Sorc);
RTMethod(void, _T2, Clear);

RTMethod(void, _T2, SetWave, void* Sorc);
RTMethod(void, _T2, SetHopSize, int HopSize);
RTMethod(void, _T2, SetPosition, int Position);
RTMethod(int , _T2, GetPosition);
RTMethod(void, _T2, SetPitch, _PMatch* Sorc);
RTMethod(void, _T2, SetUpperFreq, _T1 UFreq);

RTMethod(int , _T2, IterNextTo, int Position);
RTMethod(int , _T2, PrevTo, int Position);

#undef  _Wave
#undef  _List_Int
#undef  _List_GnrcFrame
#undef  _List_DataFrame
#undef  _STFTIterlyzer
#undef  _SinusoidalBase
#undef  _PMatch

