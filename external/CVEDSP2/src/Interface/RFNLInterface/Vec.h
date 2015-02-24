#include <RFNL.h>

#define _CDSP2_Define_FP_FD(Type, Name) \
    __CDSP2_DEF_PREFIX _C(Type, _, Float)  _C(Name, _, Float ); \
    __CDSP2_DEF_PREFIX _C(Type, _, Double) _C(Name, _, Double)

#ifndef CDSP2_VEC_H
#define CDSP2_VEC_H

typedef void (*_CDSP2_FP_Vec6P_Float) (float* , float* , float* ,
                                       float* , float* , float* , int);
typedef void (*_CDSP2_FP_Vec6P_Double)(double*, double*, double*,
                                       double*, double*, double*, int);

typedef void (*_CDSP2_FP_Vec3P_Float) (float* , float* , float* , int);
typedef void (*_CDSP2_FP_Vec3P_Double)(double*, double*, double*, int);

typedef void (*_CDSP2_FP_Vec2P_Float) (float* , float* , int);
typedef void (*_CDSP2_FP_Vec2P_Double)(double*, double*, int);

typedef void (*_CDSP2_FP_Vec2P1C_Float) (float* , float* , float , int);
typedef void (*_CDSP2_FP_Vec2P1C_Double)(double*, double*, double, int);

typedef void (*_CDSP2_FP_Vec1P1C_Float) (float* , float , int);
typedef void (*_CDSP2_FP_Vec1P1C_Double)(double*, double, int);

typedef float  (*_CDSP2_FP_Vec1P2I_Float) (float* , int, int);
typedef double (*_CDSP2_FP_Vec1P2I_Double)(double*, int, int);

#endif

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VAdd);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VSub);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VSubBy);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VMul);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VDiv);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VDivBy);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VMax);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VMin);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCAdd);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCSub);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCSubBy);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCMul);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCDiv);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCDivBy);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCMax);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VCMin);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VAbs);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VCopy);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec1P1C, CDSP2_VSet);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec1P2I, CDSP2_VMaxElmt);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec1P2I, CDSP2_VMinElmt);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec1P2I, CDSP2_VSum);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec1P2I, CDSP2_VAvg);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VSqr);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VRcp);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VRcpSqr);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VFMA);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P1C, CDSP2_VFCMA);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec6P, CDSP2_VCplxMul);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VCplxAbs);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec3P, CDSP2_VCplxArg);

_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VSin);
_CDSP2_Define_FP_FD(_CDSP2_FP_Vec2P, CDSP2_VCos);

