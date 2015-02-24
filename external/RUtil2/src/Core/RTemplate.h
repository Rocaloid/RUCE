#if _Attr == 1
    #define __Attr _T1
#endif
#if _Attr == 2
    #define __Attr _T1, _, _T2
#endif
#if _Attr == 3
    #define __Attr _T1, _, _T2, _, _T3
#endif
#if _Attr == 4
    #define __Attr _T1, _, _T2, _, _T3, _, _T4
#endif

#define _T1_TID _C(_TID, _, _T1)
#define _T2_TID _C(_TID, _, _T2)
#define _T3_TID _C(_TID, _, _T3)
#define _T4_TID _C(_TID, _, _T4)

#undef _T1_IntLike
#undef _T1_FloatLike
#if _T1_TID <= _TID_Double
    #define _T1_FloatLike
#else
    #define _T1_IntLike
#endif

#undef _T2_IntLike
#undef _T2_FloatLike
#if _T2_TID <= _TID_Double
    #define _T2_FloatLike
#else
    #define _T2_IntLike
#endif

#undef _T3_IntLike
#undef _T3_FloatLike
#if _T3_TID <= _TID_Double
    #define _T3_FloatLike
#else
    #define _T3_IntLike
#endif

#undef _T4_IntLike
#undef _T4_FloatLike
#if _T4_TID <= _TID_Double
    #define _T4_FloatLike
#else
    #define _T4_IntLike
#endif

#undef _T1_DataLen
// 1
#if _T1_TID == _TID_Char || _T1_TID == _TID_UChar
    #define _T1_DataLen 1
#endif

// 2
#if _T1_TID == _TID_Short || _T1_TID == _TID_Short || \
     _T1_TID == _TID_Int16 || _T1_TID == _TID_UInt16
    #define _T1_DataLen 2
#endif

// 4
#if _T1_TID == _TID_Float || _T1_TID == _TID_Int || _T1_TID == _TID_UInt || \
    _T1_TID == _TID_Int32 || _T1_TID == _TID_UInt32
    #define _T1_DataLen 4
#endif

// 8
#if _T1_TID == _TID_Double || _T1_TID == _TID_Int64 || _T1_TID == _TID_UInt64
    #define _T1_DataLen 8
#endif

#undef _T2_DataLen
// 1
#if _T2_TID == _TID_Char || _T2_TID == _TID_UChar
    #define _T2_DataLen 1
#endif

// 2
#if _T2_TID == _TID_Short || _T2_TID == _TID_Short || \
     _T2_TID == _TID_Int16 || _T2_TID == _TID_UInt16
    #define _T2_DataLen 2
#endif

// 4
#if _T2_TID == _TID_Float || _T2_TID == _TID_Int || _T2_TID == _TID_UInt || \
    _T2_TID == _TID_Int32 || _T2_TID == _TID_UInt32
    #define _T2_DataLen 4
#endif

// 8
#if _T2_TID == _TID_Double || _T2_TID == _TID_Int64 || _T2_TID == _TID_UInt64
    #define _T2_DataLen 8
#endif

#undef _T3_DataLen
// 1
#if _T3_TID == _TID_Char || _T3_TID == _TID_UChar
    #define _T3_DataLen 1
#endif

// 2
#if _T3_TID == _TID_Short || _T3_TID == _TID_Short || \
     _T3_TID == _TID_Int16 || _T3_TID == _TID_UInt16
    #define _T3_DataLen 2
#endif

// 4
#if _T3_TID == _TID_Float || _T3_TID == _TID_Int || _T3_TID == _TID_UInt || \
    _T3_TID == _TID_Int32 || _T3_TID == _TID_UInt32
    #define _T3_DataLen 4
#endif

// 8
#if _T3_TID == _TID_Double || _T3_TID == _TID_Int64 || _T3_TID == _TID_UInt64
    #define _T3_DataLen 8
#endif

#undef _T4_DataLen
// 1
#if _T4_TID == _TID_Char || _T4_TID == _TID_UChar
    #define _T4_DataLen 1
#endif

// 2
#if _T4_TID == _TID_Short || _T4_TID == _TID_Short || \
     _T4_TID == _TID_Int16 || _T4_TID == _TID_UInt16
    #define _T4_DataLen 2
#endif

// 4
#if _T4_TID == _TID_Float || _T4_TID == _TID_Int || _T4_TID == _TID_UInt || \
    _T4_TID == _TID_Int32 || _T4_TID == _TID_UInt32
    #define _T4_DataLen 4
#endif

// 8
#if _T4_TID == _TID_Double || _T4_TID == _TID_Int64 || _T4_TID == _TID_UInt64
    #define _T4_DataLen 8
#endif

#undef _RTClassName
#if _Attr == 1
    #define _RTClassName _C1(_ClassName, __Attr)
#else
    #define _RTClassName _C(_ClassName, __Attr)
#endif
#include _E(_RTAddress)

#undef _T1
#undef _T2
#undef _T3
#undef _T4
#undef __Attr

