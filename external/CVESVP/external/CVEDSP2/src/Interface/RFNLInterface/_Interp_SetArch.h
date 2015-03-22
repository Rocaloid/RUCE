#define __CDSP2_Interp_SetArch1(Name) \
    CDSP2_##Name##_Float  = _C(RFNL_, Name, _, __Arch, _Float); \
    CDSP2_##Name##_Double = _C(RFNL_, Name, _, __Arch, _Double)

#define __CDSP2_Interp_SetArch2(Name) \
    CDSP2_##Name##_Float_Float   = _C(RFNL_, Name, _, __Arch, _Float_Float  ); \
    CDSP2_##Name##_Float_Double  = _C(RFNL_, Name, _, __Arch, _Float_Double ); \
    CDSP2_##Name##_Double_Float  = _C(RFNL_, Name, _, __Arch, _Double_Float ); \
    CDSP2_##Name##_Double_Double = _C(RFNL_, Name, _, __Arch, _Double_Double)

__CDSP2_Interp_SetArch2(GenPeak);

__CDSP2_Interp_SetArch1(Resample_Non);
__CDSP2_Interp_SetArch1(Resample_Linear);
__CDSP2_Interp_SetArch1(Resample_Cosine);

__CDSP2_Interp_SetArch2(MapStretch_Non);
__CDSP2_Interp_SetArch2(MapStretch_Linear);
__CDSP2_Interp_SetArch2(MapStretch_Cosine);

__CDSP2_Interp_SetArch2(MatchInterp_Non);
__CDSP2_Interp_SetArch2(MatchInterp_Linear);
__CDSP2_Interp_SetArch2(MatchInterp_Cosine);

