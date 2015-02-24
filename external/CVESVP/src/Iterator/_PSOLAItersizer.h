#define _List_DataFrame _C(CDSP2_List_DataFrame, _, _T1)
#define _DataFrame _C(CDSP2_DataFrame, _, _T1)
#define _List_Int CDSP2_List_Int
#define _FWindow _C(RFNL_FWindow_Gnrc, _, _T1)

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _List_DataFrame DataList;
    _List_Int       PulseList;
    
    //Private
    void* Wave;
    _FWindow* Window;
    int LastPosition;
    int InitPosition;
};

RTMethod(void, CSVP_PSOLAItersizer, From, _RTClassName* Sorc);

RTMethod(void, CSVP_PSOLAItersizer, SetWave, void* Sorc);
RTMethod(void, CSVP_PSOLAItersizer, SetWindow, _FWindow* Sorc);
RTMethod(void, CSVP_PSOLAItersizer, SetPosition, int Position);
RTMethod(int , CSVP_PSOLAItersizer, GetPosition);

RTMethod(void, CSVP_PSOLAItersizer, RepositionFrom, int Position);

RTMethod(void, CSVP_PSOLAItersizer, IterNextTo, int Position);
RTMethod(void, CSVP_PSOLAItersizer, IterPrevTo, int Position);
RTMethod(void, CSVP_PSOLAItersizer, PrevTo, int Position);

RTMethod(void, CSVP_PSOLAItersizer, Add, _DataFrame* Sorc, int Position);

#undef  _List_DataFrame
#undef  _DataFrame
#undef  _List_Int
#undef  _FWindow

