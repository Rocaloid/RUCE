#define _TBase _C(CDSP2_Wave, _, _T1)

RClass(_RTClassName)
{
    RInherit(_TBase);
    
    //Private
    int VPosition;  //Virtual Position
    int SPosition;  //Submitted Position
};

RTMethod(void, CDSP2_InfWave, CtorSize, int Size);
RTMethod(void, CDSP2_InfWave, Resize, int Size);
RTMethod(void, CDSP2_InfWave, From, _RTClassName* Sorc);

RTMethod(int, CDSP2_InfWave, Relocate, int VPosition);
RTMethod(int, CDSP2_InfWave, Submit, int VPosition);
RTMethod(_T1*, CDSP2_InfWave, GetUnsafePtr);

RTMethod(void, CDSP2_InfWave, SetWindow, _T1* Sorc, int Size);

//Dump
RTMethod(int, CDSP2_InfWave, Dump, _T1* Dest);
//Throw
RTMethod(int, CDSP2_InfWave, Throw);
//Read
RTMethod(int, CDSP2_InfWave, Read, _T1* Dest, int LVPosition, int Size);
//Overridding Write
RTMethod(int, CDSP2_InfWave, Write, _T1* Sorc, int LVPosition, int Size);
//Additive Write
RTMethod(int, CDSP2_InfWave, Add, _T1* Sorc, int LVPosition, int Size);
//Windowed Read
RTMethod(int, CDSP2_InfWave, ReadW, _T1* Dest, int LVPosition);
//Windowed Overidding Write
RTMethod(int, CDSP2_InfWave, WriteW, _T1* Sorc, int LVPosition);
//Windowed Additive Write
RTMethod(int, CDSP2_InfWave, AddW, _T1* Sorc, int LVPosition);

