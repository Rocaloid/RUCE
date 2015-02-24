RClass(_RTClassName)
{
    RInherit(RObject);
    Array_Define(_T1, X);
    Array_Define(_T2, Y);
    
    //Private
    int LastIndex;
};

RTMethod(void, PMatch, From, _RTClassName* Sorc);
RTMethod(int , PMatch, AddPair, _T1 XVar, _T2 YVar);
RTMethod(void, PMatch, InvertTo, _RTClassName* Dest);
RTMethod(void, PMatch, Clear);
RTMethod(void, PMatch, Print);
RTMethod(_C(Transition, RTAttr_2), PMatch, Query, _T1 XVar);

