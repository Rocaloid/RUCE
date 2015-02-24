RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    _T1* Data;
    int  Size;
};

RTMethod(void, CDSP2_DataFrame, CtorSize, int Size);
RTMethod(void, CDSP2_DataFrame, Resize, int Size);
RTMethod(void, CDSP2_DataFrame, From, _RTClassName* Sorc);

RTMethod(void, CDSP2_DataFrame, Clear);

RTMethod(void, CDSP2_DataFrame, FromWave, void* Sorc, int Center,
    int HalfWidth);

