#define _List_Spectrum _C(CDSP2_List_Spectrum, _, _T1)
#define _List_Int CDSP2_List_Int
#define _Spectrum _C(CDSP2_Spectrum, _, _T1)
#define _STFTIterlyzer _C(CSVP_STFTIterlyzer, _, _T1)

/*
    Important: STFTItersizer shares identical structure to STFTIterlyzer.
*/
RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    int HopSize;
    _List_Spectrum SpecList;
    _List_Int      PulseList;
    char UseWindow;
    
    //Private
    void* Wave;
    int LastPosition;
    int InitPosition;
    _T1 WinFactor;
};

RTMethod(void, CSVP_STFTItersizer, CtorSize, int FrameSize);
RTMethod(void, CSVP_STFTItersizer, Resize, int FrameSize);
RTMethod(void, CSVP_STFTItersizer, From, _RTClassName* Sorc);
RTMethod(void, CSVP_STFTItersizer, FromIterlyzer, _STFTIterlyzer* Sorc);
RTMethod(void, CSVP_STFTItersizer, Clear);

RTMethod(void, CSVP_STFTItersizer, SetWave, void* Sorc);
RTMethod(void, CSVP_STFTItersizer, SetPosition, int Position);
RTMethod(void, CSVP_STFTItersizer, SetHopSize, int HopSize);
RTMethod(int , CSVP_STFTItersizer, GetPosition);
RTMethod(int , CSVP_STFTItersizer, GetHopSize);
RTMethod(int , CSVP_STFTItersizer, GetFrameSize);

RTMethod(void, CSVP_STFTItersizer, RepositionFrom, int Position);

RTMethod(void, CSVP_STFTItersizer, IterNextTo, int Position);
RTMethod(void, CSVP_STFTItersizer, IterPrevTo, int Position);
RTMethod(void, CSVP_STFTItersizer, PrevTo, int Position);

RTMethod(int , CSVP_STFTItersizer, Add, _Spectrum* Sorc, int Position);
RTMethod(int , CSVP_STFTItersizer, ExtractFrom, _STFTIterlyzer* Sorc);

#undef  _List_Spectrum
#undef  _List_Int
#undef  _Spectrum
#undef  _STFTIterlyzer

