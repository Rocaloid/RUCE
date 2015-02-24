//Generic Template Fast Math Header & Implementation

#undef __RFNL_Trig_CheckSign
#undef __RFNL_Trig_Indexing
#undef __RFNL_Trig_InterpRet
#undef __RFNL_Trig_NoInterpRet

#define __RFNL_Trig_SinCheckSign() \
    if(x < 0) \
        x = M_PI - x

#define __RFNL_Trig_CosCheckSign() \
    if(x < 0) \
        x = - x

#define __RFNL_Trig_Indexing() \
    x = x / M_PI / 2.0f; \
    x -= (int)x; \
    x *= RFNL_TableSize; \
    i = x

#define __RFNL_Trig_InterpRet(Table) \
    return _C(Table, _, _T1)[i] + \
          (_C(Table, _, _T1)[i + 1] - \
           _C(Table, _, _T1)[i]) * r

#define __RFNL_Trig_NoInterpRet(Table) \
    return _C(Table, _, _T1)[i]

//Sine
RTFunc(RInline _T1, RFNL_Sin_Gnrc, _T1 x)
{
    _T1 r;
    int i;
    
    __RFNL_Trig_SinCheckSign();
    __RFNL_Trig_Indexing();
    r = x - i;
    
    __RFNL_Trig_InterpRet(__RFNL_SinTable);
}

//Low precision(no interpolation) sine
RTFunc(RInline _T1, RFNL_Sin_LPrec_Gnrc, _T1 x)
{
    int i;
    
    __RFNL_Trig_SinCheckSign();
    __RFNL_Trig_Indexing();
    
    __RFNL_Trig_NoInterpRet(__RFNL_SinTable);
}

//Positive-only sine
RTFunc(RInline _T1, RFNL_PSin_Gnrc, _T1 x)
{
    _T1 r;
    int i;
    
    __RFNL_Trig_Indexing();
    r = x - i;
    
    __RFNL_Trig_InterpRet(__RFNL_SinTable);
}

//Positive-only, low precision(no interpolation) sine
RTFunc(RInline _T1, RFNL_PSin_LPrec_Gnrc, _T1 x)
{
    int i;
    
    __RFNL_Trig_Indexing();
    
    __RFNL_Trig_NoInterpRet(__RFNL_SinTable);
}

//Cosine
RTFunc(RInline _T1, RFNL_Cos_Gnrc, _T1 x)
{
    _T1 r;
    int i;
    
    __RFNL_Trig_CosCheckSign();
    __RFNL_Trig_Indexing();
    r = x - i;
    
    __RFNL_Trig_InterpRet(__RFNL_CosTable);
}

//Low precision(no interpolation) cosine
RTFunc(RInline _T1, RFNL_Cos_LPrec_Gnrc, _T1 x)
{
    int i;
    
    __RFNL_Trig_CosCheckSign();
    __RFNL_Trig_Indexing();
    
    __RFNL_Trig_NoInterpRet(__RFNL_CosTable);
}

//Positive-only cosine
RTFunc(RInline _T1, RFNL_PCos_Gnrc, _T1 x)
{
    _T1 r;
    int i;
    
    __RFNL_Trig_Indexing();
    r = x - i;
    
    __RFNL_Trig_InterpRet(__RFNL_CosTable);
}

//Positive-only, low precision(no interpolation) cosine
RTFunc(RInline _T1, RFNL_PCos_LPrec_Gnrc, _T1 x)
{
    int i;
    
    __RFNL_Trig_Indexing();
    
    __RFNL_Trig_NoInterpRet(__RFNL_CosTable);
}

//Tan

RTFunc(RInline _T1, RFNL_Tan_Gnrc, _T1 x)
{
    return RCall(RFNL_Sin_Gnrc, _T1)(x) / RCall(RFNL_Cos_Gnrc, _T1)(x);
}

RTFunc(RInline _T1, RFNL_PTan_Gnrc, _T1 x)
{
    return RCall(RFNL_PSin_Gnrc, _T1)(x) / RCall(RFNL_PCos_Gnrc, _T1)(x);
}

RTFunc(RInline _T1, RFNL_Tan_LPrec_Gnrc, _T1 x)
{
    return RCall(RFNL_Sin_LPrec_Gnrc, _T1)(x) / 
           RCall(RFNL_Cos_LPrec_Gnrc, _T1)(x);
}

RTFunc(RInline _T1, RFNL_PTan_LPrec_Gnrc, _T1 x)
{
    return RCall(RFNL_PSin_LPrec_Gnrc, _T1)(x) / 
           RCall(RFNL_PCos_LPrec_Gnrc, _T1)(x);
}

#include "_FMath_Pk_Gnrc.h"

