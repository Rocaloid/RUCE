//Generic Template Fast Window Generator Header

#undef SizeFuncType
#undef ValuFuncType
#define SizeFuncType _C(RFNL_FWSizeFunc, _, _T1)
#define ValuFuncType _C(RFNL_FWValuFunc, _, _T1)

typedef int (* SizeFuncType)(_T1 Var);
typedef _T1 (* ValuFuncType)(_T1 Var, int Size);

RClass(_RTClassName)
{
    RInherit(RObject);
    _T1 MinVar;
    _T1 MaxVar;
    _T1 Interval;
    
    SizeFuncType SizeFunc;
    ValuFuncType ValuFunc;
    
    int MaxSize;
    Array_Define(_T1*, Buff);
};

RTMethod(void, RFNL_FWindow_Gnrc, SetPara,
    _T1 MinVar, _T1 MaxVar, _T1 Interval);
RTMethod(void, RFNL_FWindow_Gnrc, SetFunc,
    SizeFuncType SizeFunc, ValuFuncType ValuFunc);
RTMethod(int , RFNL_FWindow_Gnrc, Initialize);
RTMethod(int , RFNL_FWindow_Gnrc, Generate, _T1* Dest, _T1 Var);
RTMethod(void, RFNL_FWindow_Gnrc, Reset);

//Common Windows
RTFunc(_T1, RFNL_Hanning_Valu_Gnrc, _T1 Var, int Size);
RTFunc(int, RFNL_Hanning_Size_Gnrc, _T1 Var);
RTFunc(_T1, RFNL_Hamming_Valu_Gnrc, _T1 Var, int Size);
RTFunc(int, RFNL_Hamming_Size_Gnrc, _T1 Var);
RTFunc(_T1, RFNL_Blackman_Valu_Gnrc, _T1 Var, int Size);
RTFunc(int, RFNL_Blackman_Size_Gnrc, _T1 Var);

//Instant Window
RTFunc(void, RFNL_Hanning_Gnrc, _T1* Dest, _T1* Sorc, int Size);
RTFunc(void, RFNL_Hamming_Gnrc, _T1* Dest, _T1* Sorc, int Size);
RTFunc(void, RFNL_Blackman_Gnrc, _T1* Dest, _T1* Sorc, int Size);

RTFunc(void, RFNL_GenHanning_Gnrc, _T1* Dest, int Size);
RTFunc(void, RFNL_GenHamming_Gnrc, _T1* Dest, int Size);
RTFunc(void, RFNL_GenBlackman_Gnrc, _T1* Dest, int Size);

