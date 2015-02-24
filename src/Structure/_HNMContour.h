RClass(_RTClassName)
{
    RInherit(RObject);
    
    //Public
    int  Size;
    
    _T1* Hmnc;
    _T1* Noiz;
};

RTMethod(void, CSVP_HNMContour, CtorSize, int Size);
RTMethod(void, CSVP_HNMContour, Resize, int Size);
RTMethod(void, CSVP_HNMContour, From, _RTClassName* Sorc);
RTMethod(void, CSVP_HNMContour, InterpFrom, _RTClassName* Sorc1,
    _RTClassName* Sorc2, _T1 Ratio);

RTMethod(void, CSVP_HNMContour, Clear);

