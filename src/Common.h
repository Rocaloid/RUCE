#ifndef RUCE_COMMON_H
#define RUCE_COMMON_H

#define Real Float
#define _Wave CDSP2_Wave_Float
#define _Spectrum CDSP2_Spectrum_Float
#define _PSOLAItersizer CSVP_PSOLAItersizer_Float
#define _HNMItersizer CSVP_HNMItersizer_Float
#define _FWindow_T RFNL_FWindow_Gnrc_Float
#define _FWindow CDSP2_FWindow_Float
#define _DataFrame CDSP2_DataFrame_Float
#define _HNMFrame CSVP_HNMFrame_Float
#define _Sinusoid CSVP_Sinusoid_Float
#define _HNMContour CSVP_HNMContour_Float
#define _List_DataFrame CDSP2_List_DataFrame_Float
#define _List_HNMFrame CSVP_List_HNMFrame_Float
#define _List_Int CDSP2_List_Int
#define _PMatch PMatch_Float_Float
#define _Transition Transition_Float_Float
#define _MapStretch CDSP2_MapStretch_Linear_Float_Float

#define WINSIZE 2048
#define Verbose(...) printf(__VA_ARGS__)

#endif

