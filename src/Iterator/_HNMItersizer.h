#define _Wave _C(CDSP2_Wave, _, _T1)
#define _List_Int CDSP2_List_Int
#define _List_HNMFrame _C(CSVP_List_HNMFrame, _, _T1)
#define _HNMFrame _C(CSVP_HNMFrame, _, _T1)
#define _DataFrame _C(CDSP2_DataFrame, _, _T1)
#define _SinusoidItersizer _C(CSVP_SinusoidItersizer, _, _T1)
#define _STFTItersizer _C(CSVP_STFTItersizer, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _List_Int      PulseList;
    _List_HNMFrame HNMList;
    
    struct
    {
        char PhaseControl;
    } Option;
    
    //Private
    char SeparateNoiz;
    _SinusoidItersizer* SubsizerS;
    _STFTItersizer* SubsizerF;
    int HopSize;
    int Size;
    int InitPosition;
};

RTMethod(void, CSVP_HNMItersizer, CtorSize, int Size);
RTMethod(void, CSVP_HNMItersizer, Resize, int Size);
RTMethod(void, CSVP_HNMItersizer, From, _RTClassName* Sorc);
RTMethod(void, CSVP_HNMItersizer, Clear);

RTMethod(void, CSVP_HNMItersizer, SetWave, void* Sorc);
RTMethod(void, CSVP_HNMItersizer, SetNoizWave, void* Sorc);
RTMethod(void, CSVP_HNMItersizer, SetInitPhase, _DataFrame* Sorc);
RTMethod(void, CSVP_HNMItersizer, SetPosition, int Position);
RTMethod(void, CSVP_HNMItersizer, SetHopSize, int HopSize);
RTMethod(int , CSVP_HNMItersizer, GetPosition);

RTMethod(void, CSVP_HNMItersizer, IterNextTo, int Position);
RTMethod(void, CSVP_HNMItersizer, PrevTo, int Position);

RTMethod(void, CSVP_HNMItersizer, Add, _HNMFrame* Sorc, int Position);
RTMethod(void, CSVP_HNMItersizer, AddPhase, _DataFrame* Sorc, int Position);

#undef  _Wave
#undef  _List_Int
#undef  _List_HNMFrame
#undef  _HNMFrame
#undef  _DataFrame
#undef  _SinusoidItersizer
#undef  _STFTItersizer

