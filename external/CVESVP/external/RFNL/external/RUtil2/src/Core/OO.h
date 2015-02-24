#ifndef RUTIL2_OO_H
#define RUTIL2_OO_H

#include "MacroUtil.h"
#include "../Structure/Array.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <limits.h>

extern int __ClassID__;

//Rocaloid-style type aliases. Never use long!
typedef float               Float;
typedef double              Double;
typedef int                 Int;
typedef unsigned int        UInt;
typedef short int           Short;
typedef unsigned short int  UShort;
typedef char                Char;
typedef unsigned char       UChar;
typedef int16_t             Int16;
typedef uint16_t            UInt16;
typedef int32_t             Int32;
typedef uint32_t            UInt32;
typedef int64_t             Int64;
typedef uint64_t            UInt64;
#define _TID_Float          0
#define _TID_Double         1
#define _TID_Int            2
#define _TID_UInt           3
#define _TID_Short          4
#define _TID_UShort         5
#define _TID_Char           6
#define _TID_UChar          7
#define _TID_Int16          8
#define _TID_UInt16         9
#define _TID_Int32          10
#define _TID_UInt32         11
#define _TID_Int64          12
#define _TID_UInt64         13
#define _Max_Float          INFINITY
#define _Max_Double         INFINITY
#define _Max_Int            INT_MAX
#define _Max_UInt           UINT_MAX
#define _Max_Short          SHRT_MAX
#define _Max_UShort         USHRT_MAX
#define _Max_Char           CHAR_MAX
#define _Max_UChar          UCHAR_MAX
#define _Max_Int16          SHRT_MAX
#define _Max_UInt16         USHRT_MAX
#define _Max_Int32          LONG_MAX
#define _Max_UInt32         ULONG_MAX
#define _Max_Int64          LLONG_MAX
#define _Max_UInt64         ULLONG_MAX
#define _Min_Float          (- INFINITY)
#define _Min_Double         (- INFINITY)
#define _Min_Int            INT_MIN
#define _Min_UInt           0
#define _Min_Short          SHRT_MIN
#define _Min_UShort         0
#define _Min_Char           CHAR_MIN
#define _Min_UChar          0
#define _Min_Int16          SHRT_MIN
#define _Min_UInt16         0
#define _Min_Int32          LONG_MIN
#define _Min_UInt32         0
#define _Min_Int64          LLONG_MIN
#define _Min_UInt64         0

//Automatic constructor/destructor.
typedef void (*__CDtorFunc)(void*);
Array_Extern(__CDtorFunc, __AutoCtor);
Array_Extern(__CDtorFunc, __AutoDtor);
void __AutoCDtor_Init();
void __AutoCDtor_Exit();
void __RNew(__CDtorFunc Ctor, ...);
void __RDelete(void* a, ...);
#define RNew(Type, ...) \
    __RNew(__AutoCtor[_C2(__ClassID_, Type, __)], __VA_ARGS__, (void*)(- 1))
#define RDelete(...) __RDelete(__VA_ARGS__, (void*)(- 1))

#define RClass(Name) \
    typedef struct Name Name; \
    extern int _C2(__ClassID_, Name, __); \
    void _C1(Name, _Ctor)(Name* This); \
    void _C1(Name, _Dtor)(Name* This); \
    struct Name

#define RInherit(BaseClass) \
    BaseClass _Base

#define RCtor(Name) \
    int _C2(__ClassID_, Name, __); \
    void __attribute__  ((constructor (499))) _C2(__, Name, _ClassInit__)() \
    { \
        if(__ClassID__ == 0) \
            __AutoCDtor_Init(); \
        _C2(__ClassID_, Name, __) = __ClassID__ ++; \
        Array_Push(__CDtorFunc, __AutoCtor, (__CDtorFunc)_C1(Name, _Ctor)); \
        Array_Push(__CDtorFunc, __AutoDtor, (__CDtorFunc)_C1(Name, _Dtor)); \
    }\
    void _C1(Name, _Ctor)(Name* This)

#define RDtor(Name) \
    void _C1(Name, _Dtor)(Name* This)

#define RInit(Name) \
    ((RObject*)This) -> ClassID = _C2(__ClassID_, Name, __);

#define MyBase(Name) \
    ((Name*)This)

//Obsoleted!
#define RInterface_Start() \
    if(0) \
        __ClassID__ = __ClassID__

//Obsoleted!
#define RInterface_Add(Class, Method) \
    else if(This -> ClassID == _C2(__ClassID_, Class, __)) \
        return _C2(Class, _, Method)

#define RInterface_Define(RetType, Name, ...) \
    typedef RetType (*_C(__Itfc_, Name, _CallType))(__VA_ARGS__); \
    Array_Extern(int, _C(__Itfc_, Name, _ClassID)); \
    Array_Extern(_C(__Itfc_, Name, _, CallType), _C(__Itfc_, Name, _CallPtr)); \
    RetType Name(__VA_ARGS__);

#define RInterface_DefCall(Name, ...) \
    int i;\
    RObject* RThis = (RObject*) This; \
    for(i = 0; i <= _C(__Itfc_, Name, _ClassID_Index); i ++) \
        if(_C(__Itfc_, Name, _ClassID)[i] == RThis -> ClassID) \
            return _C(__Itfc_, Name, _CallPtr)[i](__VA_ARGS__); \
    RDebugPrint(Name, ": incompatible type of object.")

#define RInterface_Make(Name) \
    Array_Define(int, _C(__Itfc_, Name, _ClassID)); \
    Array_Define(_C(__Itfc_, Name, _, CallType), _C(__Itfc_, Name, _CallPtr)); \
    void __attribute__ ((constructor (500))) _C(__Itfc_, Name, _Ctor)() \
    { \
        Array_Ctor(int, _C(__Itfc_, Name, _ClassID)); \
        Array_Ctor(_C(__Itfc_, Name, _, CallType), \
                   _C(__Itfc_, Name, _CallPtr)); \
    } \
    void __attribute__ ((destructor)) _C(__Itfc_, Name, _Dtor)() \
    { \
        Array_Dtor(int, _C(__Itfc_, Name, _ClassID)); \
        Array_Dtor(_C(__Itfc_, Name, _, CallType), \
                   _C(__Itfc_, Name, _CallPtr)); \
    }

#define RInterface_AddMethod(IName, MName, ClsName) \
    void __attribute__ ((constructor (550))) \
        _C(__Itfc_, IName, _Add_, ClsName)() \
    { \
        Array_Push(int, _C(__Itfc_, IName, _ClassID), \
                        _C(__ClassID_, ClsName, __)); \
        Array_Push(_C(__Itfc_, IName, _, CallType), \
                   _C(__Itfc_, IName, _CallPtr), \
                   (_C(__Itfc_, IName, _, CallType))MName); \
    }

#define RTMethod(Ret, Name, Method, ...) \
    Ret _C(Name, _, __Attr, _, Method) \
        (_C(Name, _, __Attr)* This, ##__VA_ARGS__)

#define RTFunc(Ret, Method, ...) \
    Ret _C(Method, _, __Attr) \
        (__VA_ARGS__)

#define RTAttr_1 _, _T1
#define RTAttr_2 _, _T1, _, _T2
#define RTAttr_3 _, _T1, _, _T2, _, _T3
#define RTAttr_4 _, _T1, _, _T2, _, _T3, _, _T4

#define RCall(Class, Method) _C2(Class, _, Method)

#define RDebugPrint(Field, Str) \
    printf("[Runtime] " _S(Field) "_" Str "\n");

#ifdef WITH_ASSERT_ABORT
    #define _DUMMY_ABORT abort
#else
    #define _DUMMY_ABORT() (void)0
#endif

#ifndef NDEBUG
    #define RAssert(a, ...) \
        (void)((!(a)) ?  ( \
            ( \
            fprintf(stderr, \
                "RAssert failed: %s:%d, %s(), at \'%s\' " __VA_ARGS__ "\n", \
                __FILE__, __LINE__, __func__, _S(a)), \
            _DUMMY_ABORT(), \
            NULL)) : NULL)
#else
    #define RAssert(a, ...)
#endif

#define Likely(x)        __builtin_expect(!!(x), 1)
#define Unlikely(x)      __builtin_expect(!!(x), 0)

#ifndef RInline
    /* little macro so inline keyword works */
    #if defined(_MSC_VER)
        #define RInline static __forceinline
    #else
        #if (defined(__APPLE__) && defined(__ppc__))
    /* static inline __attribute__ here breaks osx ppc gcc42 build */
            #define RInline static __attribute__((always_inline))
        #else
            #define RInline static inline __attribute__((always_inline))
        #endif
    #endif
#endif
/*
    Class: RObject
    
        The base class of all RClasses.
*/
RClass(RObject)
{
    /*
        Variable: ClassID
        
            Stores the unique ID representing the RClass of this object.
    */
    int ClassID;
};

#endif //RUTIL2_OO_H

#undef _RTAddress
#undef _ClassName
#undef _Attr

