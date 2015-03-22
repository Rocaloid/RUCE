#define _List_Spectrum _C(CDSP2_List_Spectrum, _, _T1)
#define _List_Int CDSP2_List_Int
#define _Spectrum _C(CDSP2_Spectrum, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    int HopSize;
    _List_Spectrum SpecList;
    _List_Int      PulseList;
    
    //Private
    char UseWindow;
    void* Wave;
    int LastPosition;
    int InitPosition;
};

RTMethod(void, CSVP_STFTIterlyzer, CtorSize, int FrameSize);
RTMethod(void, CSVP_STFTIterlyzer, Resize, int FrameSize);
RTMethod(void, CSVP_STFTIterlyzer, From, _RTClassName* Sorc);
RTMethod(void, CSVP_STFTIterlyzer, Clear);

RTMethod(void, CSVP_STFTIterlyzer, SetWave, void* Sorc);
RTMethod(void, CSVP_STFTIterlyzer, SetPosition, int Position);
RTMethod(void, CSVP_STFTIterlyzer, SetHopSize, int HopSize);
RTMethod(int , CSVP_STFTIterlyzer, GetPosition);
RTMethod(int , CSVP_STFTIterlyzer, GetHopSize);
RTMethod(int , CSVP_STFTIterlyzer, GetFrameSize);

RTMethod(void, CSVP_STFTIterlyzer, IterNextTo, int Position);
RTMethod(void, CSVP_STFTIterlyzer, IterPrevTo, int Position);
RTMethod(void, CSVP_STFTIterlyzer, PrevTo, int Position);

RTMethod(int , CSVP_STFTIterlyzer, Extract, _Spectrum* Dest, int Index);
RTMethod(int , CSVP_STFTIterlyzer, Fetch, _Spectrum* Dest, int Index);

#undef  _List_Spectrum
#undef  _List_Int
#undef  _Spectrum

