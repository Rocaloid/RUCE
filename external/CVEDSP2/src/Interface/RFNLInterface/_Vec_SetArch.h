#define __CDSP2_Vec_SetArch(Name) \
    CDSP2_##Name##_Float  = _C(RFNL_, Name, _, __Arch, _Float); \
    CDSP2_##Name##_Double = _C(RFNL_, Name, _, __Arch, _Double)

__CDSP2_Vec_SetArch(VAdd);
__CDSP2_Vec_SetArch(VSub);
__CDSP2_Vec_SetArch(VSubBy);
__CDSP2_Vec_SetArch(VMul);
__CDSP2_Vec_SetArch(VDiv);
__CDSP2_Vec_SetArch(VDivBy);

__CDSP2_Vec_SetArch(VMax);
__CDSP2_Vec_SetArch(VMin);

__CDSP2_Vec_SetArch(VCAdd);
__CDSP2_Vec_SetArch(VCSub);
__CDSP2_Vec_SetArch(VCSubBy);
__CDSP2_Vec_SetArch(VCMul);
__CDSP2_Vec_SetArch(VCDiv);
__CDSP2_Vec_SetArch(VCDivBy);

__CDSP2_Vec_SetArch(VCMax);
__CDSP2_Vec_SetArch(VCMin);

__CDSP2_Vec_SetArch(VAbs);
__CDSP2_Vec_SetArch(VCopy);
__CDSP2_Vec_SetArch(VSet);

__CDSP2_Vec_SetArch(VMaxElmt);
__CDSP2_Vec_SetArch(VMinElmt);

__CDSP2_Vec_SetArch(VSum);
__CDSP2_Vec_SetArch(VAvg);

__CDSP2_Vec_SetArch(VSqr);
__CDSP2_Vec_SetArch(VRcp);
__CDSP2_Vec_SetArch(VRcpSqr);

__CDSP2_Vec_SetArch(VFMA);
__CDSP2_Vec_SetArch(VFCMA);

__CDSP2_Vec_SetArch(VCplxMul);
__CDSP2_Vec_SetArch(VCplxAbs);
__CDSP2_Vec_SetArch(VCplxArg);

__CDSP2_Vec_SetArch(VSin);
__CDSP2_Vec_SetArch(VCos);

