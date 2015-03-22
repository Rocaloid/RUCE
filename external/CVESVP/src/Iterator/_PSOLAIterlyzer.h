#define _InfWave _C(CDSP2_InfWave, _, _T1)
#define _List_Int CDSP2_List_Int
#define _STFTIterlyzer _C(CSVP_STFTIterlyzer, _, _T1)
#define _STFTItersizer _C(CSVP_STFTItersizer, _, _T1)
#define _PMatch _C(PMatch_, _T1, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _List_Int PulseList;
    
    //Private
    void* Wave;
    _T1* Window;
    _InfWave* LPFWave;
    _STFTIterlyzer* Sublyzer;
    _STFTItersizer* Subsizer;
    int LastPosition;
    int InitPosition;
    int Boundary;
    
    _PMatch* RefF0;
    _T1 LPF;
    int PeakSign;
};

RTMethod(void, CSVP_PSOLAIterlyzer, From, _RTClassName* Sorc);
RTMethod(void, CSVP_PSOLAIterlyzer, Clear);

RTMethod(void, CSVP_PSOLAIterlyzer, SetWave, void* Sorc);
RTMethod(void, CSVP_PSOLAIterlyzer, SetPosition, int Position);
RTMethod(int , CSVP_PSOLAIterlyzer, GetPosition);
RTMethod(void, CSVP_PSOLAIterlyzer, SetPitch, _PMatch* Sorc);
RTMethod(void, CSVP_PSOLAIterlyzer, SetBound, _T1 Position);

RTMethod(int , CSVP_PSOLAIterlyzer, PreAnalysisTo, int Position);

RTMethod(int , CSVP_PSOLAIterlyzer, IterNextTo, int Position);
RTMethod(int , CSVP_PSOLAIterlyzer, PrevTo, int Position);

RTMethod(int , CSVP_PSOLAIterlyzer, Extract, int Index);
RTMethod(int , CSVP_PSOLAIterlyzer, Fetch, int Index);

#undef  _InfWave
#undef  _List_Int
#undef  _STFTIterlyzer
#undef  _STFTItersizer
#undef  _PMatch

