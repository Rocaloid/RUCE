RInterface_Define(void, _C(CDSP2_IWave_, _T1, _Resize), void*, int);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _From), void*, void*);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _SetWindow), void*, _T1*, int);
RInterface_Define(_T1*, _C(CDSP2_IWave_, _T1, _GetUnsafePtr), void*);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _Read), void*, _T1*, int, int);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _Write), void*, _T1*, int, int);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _Add), void*, _T1*, int, int);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _ReadW), void*, _T1*, int);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _WriteW), void*, _T1*, int);
RInterface_Define(void, _C(CDSP2_IWave_, _T1, _AddW), void*, _T1*, int);

void _C(CDSP2_IWave_, _T1, _FWRead)
    (void* This, _T1* Dest, _C(RFNL_FWindow_Gnrc, _, _T1)* Wind,
    int Position, int Size);
void _C(CDSP2_IWave_, _T1, _FWWrite)
    (void* This, _T1* Sorc, _C(RFNL_FWindow_Gnrc, _, _T1)* Wind,
    int Position, int Size);
void _C(CDSP2_IWave_, _T1, _FWAdd)
    (void* This, _T1* Sorc, _C(RFNL_FWindow_Gnrc, _, _T1)* Wind,
    int Position, int Size);

RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _T1* Data;
    int  Size;
    int  SampleRate;
    _T1* Window;
    int  WinSize;
    
    //Private
    _T1* WinBuff;
};

RTMethod(void, CDSP2_Wave, CtorSize, int Size);
RTMethod(void, CDSP2_Wave, Resize, int Size);
RTMethod(void, CDSP2_Wave, From, _RTClassName* Sorc);

RTMethod(void, CDSP2_Wave, SetWindow, _T1* Sorc, int Size);

RTMethod(_T1*, CDSP2_Wave, GetUnsafePtr);
//Read
RTMethod(void, CDSP2_Wave, Read, _T1* Dest, int LBound, int Size);
//Overridding Write
RTMethod(void, CDSP2_Wave, Write, _T1* Sorc, int LBound, int Size);
//Additive Write
RTMethod(void, CDSP2_Wave, Add, _T1* Sorc, int LBound, int Size);
//Windowed Read
RTMethod(void, CDSP2_Wave, ReadW, _T1* Dest, int LBound);
//Windowed Overidding Write
RTMethod(void, CDSP2_Wave, WriteW, _T1* Sorc, int LBound);
//Windowed Additive Write
RTMethod(void, CDSP2_Wave, AddW, _T1* Sorc, int LBound);

RTMethod(int, CDSP2_Wave, FromFile, String* Sorc);
RTMethod(int, CDSP2_Wave, ToFile, String* Dest);

