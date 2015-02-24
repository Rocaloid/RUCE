#define MyType _C(_ClassName, _T3, _, _T1)
#define MyName _C1(_ClassName, _T3)

RClass(MyType)
{
    RInherit(RObject);

    //Public
    Array_Define(_T2, Frames);
    #ifndef _CDSP2_NOSIZE
    int FrameSize;
    #endif
    int FrameNum;
};

#ifndef _CDSP2_NOSIZE
RTMethod(void, MyName, CtorSize, int FrameNum, int FrameSize);
RTMethod(void, MyName, Resize, int FrameNum, int FrameSize);
#else
RTMethod(void, MyName, CtorSize, int FrameNum);
RTMethod(void, MyName, Resize, int FrameNum);
#endif
RTMethod(void, MyName, From, MyType* Sorc);
RTMethod(void, MyName, Clear);

RTMethod(void, MyName, Fetch, _T2* Dest, int Index);
RTMethod(void, MyName, Extract, _T2* Dest, int Index);
RTMethod(void, MyName, Remove, int Index);
RTMethod(void, MyName, RemoveRange, int LIndex, int HIndex);

RTMethod(void, MyName, Add, _T2* Sorc, int Index);

#undef  MyType
#undef  MyName

