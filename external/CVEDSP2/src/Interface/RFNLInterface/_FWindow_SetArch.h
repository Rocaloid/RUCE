#define __CDSP2_FWindow_SetArch(Name) \
    CDSP2_FWindow_Float_##Name  = _C(RFNL_FWindow_, __Arch, _Float_, Name); \
    CDSP2_FWindow_Double_##Name = _C(RFNL_FWindow_, __Arch, _Double_, Name)

__CDSP2_FWindow_SetArch(SetPara);
__CDSP2_FWindow_SetArch(SetFunc);
__CDSP2_FWindow_SetArch(Initialize);
__CDSP2_FWindow_SetArch(Generate);
__CDSP2_FWindow_SetArch(Reset);

#define __CDSP2_InstWindow_SetArch(Name) \
    CDSP2_##Name##_Float  = _C(RFNL_, Name, _, __Arch, _Float ); \
    CDSP2_##Name##_Double = _C(RFNL_, Name, _, __Arch, _Double)

__CDSP2_InstWindow_SetArch(Hanning);
__CDSP2_InstWindow_SetArch(Hamming);
__CDSP2_InstWindow_SetArch(Blackman);

__CDSP2_InstWindow_SetArch(GenHanning);
__CDSP2_InstWindow_SetArch(GenHamming);
__CDSP2_InstWindow_SetArch(GenBlackman);

