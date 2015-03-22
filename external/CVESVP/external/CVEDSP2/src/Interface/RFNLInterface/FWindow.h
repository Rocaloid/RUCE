#include <RFNL.h>

#define _CDSP2_Define_FP_FWindow(Type, Name) \
     __CDSP2_DEF_PREFIX _C(Type, _, Float ) _C(CDSP2_FWindow_Float , _, Name); \
     __CDSP2_DEF_PREFIX _C(Type, _, Double) _C(CDSP2_FWindow_Double, _, Name)

#ifndef CDSP2_FWINDOW_1
#define CDSP2_FWINDOW_1

typedef void (*_CDSP2_FP_FW3C_Float)
    (RFNL_FWindow_Gnrc_Float* , float , float , float );
typedef void (*_CDSP2_FP_FW3C_Double)
    (RFNL_FWindow_Gnrc_Double*, double, double, double);

typedef void (*_CDSP2_FP_FW2F_Float)
    (RFNL_FWindow_Gnrc_Float* ,RFNL_FWSizeFunc_Float , RFNL_FWValuFunc_Float );
typedef void (*_CDSP2_FP_FW2F_Double)
    (RFNL_FWindow_Gnrc_Double*,RFNL_FWSizeFunc_Double, RFNL_FWValuFunc_Double);

typedef int (*_CDSP2_FP_FWVD_Float) (RFNL_FWindow_Gnrc_Float* );
typedef int (*_CDSP2_FP_FWVD_Double)(RFNL_FWindow_Gnrc_Double*);

typedef void (*_CDSP2_FP_FWVDVR_Float) (RFNL_FWindow_Gnrc_Float* );
typedef void (*_CDSP2_FP_FWVDVR_Double)(RFNL_FWindow_Gnrc_Double*);

typedef int (*_CDSP2_FP_FW1P1C_Float)
    (RFNL_FWindow_Gnrc_Float* , float* , float );
typedef int (*_CDSP2_FP_FW1P1C_Double)
    (RFNL_FWindow_Gnrc_Double*, double*, double);

#endif

_CDSP2_Define_FP_FWindow(_CDSP2_FP_FW3C, SetPara);
_CDSP2_Define_FP_FWindow(_CDSP2_FP_FW2F, SetFunc);
_CDSP2_Define_FP_FWindow(_CDSP2_FP_FWVD, Initialize);
_CDSP2_Define_FP_FWindow(_CDSP2_FP_FW1P1C, Generate);
_CDSP2_Define_FP_FWindow(_CDSP2_FP_FWVDVR, Reset);

#define _CDSP2_Define_FP_InstWindow(Type, Name) \
    __CDSP2_DEF_PREFIX _C(Type, _, Float ) _C(CDSP2_, Name, _Float); \
    __CDSP2_DEF_PREFIX _C(Type, _, Double) _C(CDSP2_, Name, _Double)

#ifndef CDSP2_FWINDOW_2
#define CDSP2_FWINDOW_2

typedef void (*_CDSP2_FP_FWIW2P_Float) (float* , float* , int);
typedef void (*_CDSP2_FP_FWIW2P_Double)(double*, double*, int);

typedef void (*_CDSP2_FP_FWIW1P_Float) (float* , int);
typedef void (*_CDSP2_FP_FWIW1P_Double)(double*, int);

#endif

_CDSP2_Define_FP_InstWindow(_CDSP2_FP_FWIW2P, Hanning);
_CDSP2_Define_FP_InstWindow(_CDSP2_FP_FWIW2P, Hamming);
_CDSP2_Define_FP_InstWindow(_CDSP2_FP_FWIW2P, Blackman);

_CDSP2_Define_FP_InstWindow(_CDSP2_FP_FWIW1P, GenHanning);
_CDSP2_Define_FP_InstWindow(_CDSP2_FP_FWIW1P, GenHamming);
_CDSP2_Define_FP_InstWindow(_CDSP2_FP_FWIW1P, GenBlackman);
