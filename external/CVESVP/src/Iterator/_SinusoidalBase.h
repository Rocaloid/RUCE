#define _List_Int CDSP2_List_Int
#define _STFTIterlyzer _C(CSVP_STFTIterlyzer, _, _T1)
#define _PMatch _C(PMatch_, _T1, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _List_Int PulseList;
    Array_Define(_T1, FreqList);
    
    //Private
    void* Wave;
    int Size;
    
    int LastPosition;
    int InitPosition;
    _PMatch* RefF0;
    
    _STFTIterlyzer* Sublyzer0;
    _STFTIterlyzer* Sublyzer1;
};

RTMethod(void, CSVP_SinusoidalBase, CtorSize, int Size);
RTMethod(void, CSVP_SinusoidalBase, Resize, int Size);
RTMethod(void, CSVP_SinusoidalBase, From, _RTClassName* Sorc);
RTMethod(void, CSVP_SinusoidalBase, Clear);

RTMethod(void, CSVP_SinusoidalBase, SetWave, void* Sorc);
RTMethod(void, CSVP_SinusoidalBase, SetHopSize, int HopSize);
RTMethod(void, CSVP_SinusoidalBase, SetPosition, int Position);
RTMethod(int , CSVP_SinusoidalBase, GetPosition);
RTMethod(void, CSVP_SinusoidalBase, SetPitch, _PMatch* Sorc);

RTMethod(int , CSVP_SinusoidalBase, IterNextTo, int Position);
RTMethod(int , CSVP_SinusoidalBase, PrevTo, int Position);

#undef  _List_Int
#undef  _STFTIterlyzer
#undef  _PMatch

