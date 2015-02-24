//Generic Template Packed Fast Math Header & Implementation

#define __RFNL_Define_Pk4Trig_Gnrc(PackName, BaseName) \
    RTFunc(RInline void, _C(PackName, _, Gnrc), _T1* Dest, _T1* Sorc) \
    { \
        Dest[0] = _C(BaseName, _Gnrc_, _T1)(Sorc[0]); \
        Dest[1] = _C(BaseName, _Gnrc_, _T1)(Sorc[1]); \
        Dest[2] = _C(BaseName, _Gnrc_, _T1)(Sorc[2]); \
        Dest[3] = _C(BaseName, _Gnrc_, _T1)(Sorc[3]); \
    }

__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4Sin, RFNL_Sin);
__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4PSin, RFNL_PSin);
__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4Sin_LPrec, RFNL_Sin_LPrec);
__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4PSin_LPrec, RFNL_PSin_LPrec);

__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4Cos, RFNL_Cos);
__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4PCos, RFNL_PCos);
__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4Cos_LPrec, RFNL_Cos_LPrec);
__RFNL_Define_Pk4Trig_Gnrc(RFNL_Pk4PCos_LPrec, RFNL_PCos_LPrec);

//Packed tangent function is not implemented yet.

