#ifndef RUTIL2_ARRAY_H
#define RUTIL2_ARRAY_H

#include <stdlib.h>
#define Array_Addition 96

// RoundUpToMultipleOf8
// (value + roundto) & (~ (roundto - 1))
#define __Round8(v) \
    ((v + 7) & 0xFFFFFFF8)

#define _ProtoArray_Resize(Type, Array, IndexNumber, Size, NewSize) \
    do{ \
        (Array) = (Type*)realloc((Array), __Round8(sizeof(Type) * (NewSize))); \
        (Size) = NewSize; \
        if((NewSize) <= (IndexNumber)) \
            (IndexNumber) = (NewSize) - 1; \
    }while(0)

#define _ProtoArray_Push(Type, Array, IndexNumber, Size, Data) \
    do{ \
        if((IndexNumber) > (Size) - 2) \
        { \
            (Array) = (Type*)realloc((Array), __Round8(sizeof(Type) * \
                      ((Array_Addition) + (Size)))); \
            (Size) += Array_Addition; \
        } \
        (Array)[++ IndexNumber] = Data; \
    }while(0)

#define _ProtoArray_PushNull(Type, Array, IndexNumber, Size) \
    do{ \
        if((IndexNumber) > (Size) - 2) \
        { \
            (Array) = (Type*)realloc((Array), __Round8(sizeof(Type) * \
                      ((Array_Addition) + (Size)))); \
            (Size) += Array_Addition; \
        } \
        (IndexNumber) ++; \
    }while(0)

#define _ProtoArray_Pop(Type, Array, IndexNumber, Size) \
    ((Array)[IndexNumber --])

#define _ProtoArray_Insert(Type, Array, IndexNumber, Size, Index, Data) \
    do{ \
        int Array_i; \
        if((IndexNumber) > (Size) - 2) \
        { \
            (Array) = (Type*)realloc((Array), __Round8(sizeof(Type) * \
                      ((Array_Addition) + (Size)))); \
            (Size) += Array_Addition; \
        } \
        for(Array_i = (IndexNumber) + 1; Array_i > (Index); Array_i --) \
            (Array)[Array_i] = (Array)[Array_i - 1]; \
        (Array)[Index] = Data; \
        (IndexNumber) ++; \
    }while(0)

#define _ProtoArray_InsertNull(Type, Array, IndexNumber, Size, Index) \
    do{ \
        int Array_i; \
        if((IndexNumber) > (Size) - 2) \
        { \
            (Array) = (Type*)realloc((Array), __Round8(sizeof(Type) * \
                      ((Array_Addition) + (Size)))); \
            (Size) += Array_Addition; \
        } \
        for(Array_i = (IndexNumber) + 1; Array_i > (Index); Array_i --) \
            (Array)[Array_i] = (Array)[Array_i - 1]; \
        (IndexNumber) ++; \
    }while(0)

#define _ProtoArray_Remove(Type, Array, IndexNumber, Size, Index) \
    do{ \
        int Array_i; \
        for(Array_i = (Index); Array_i < (IndexNumber); Array_i ++) \
            (Array)[Array_i] = (Array)[Array_i + 1]; \
        (IndexNumber) --; \
    }while(0)

#define _ProtoArray_RemoveRange(Type, Array, IndexNumber, Size, \
    LIndex, HIndex) \
    do{ \
        int Array_i; \
        int Array_RemoveAmount = (HIndex) - (LIndex) + 1; \
        for(Array_i = (LIndex); Array_i < (IndexNumber) - (HIndex - LIndex); \
            Array_i ++) \
            (Array)[Array_i] = (Array)[Array_i + Array_RemoveAmount]; \
        (IndexNumber) -= Array_RemoveAmount; \
    }while(0)

#define Array_Define(Type, Name) \
    Type* Name; \
    int _C1(Name, _Index); \
    int _C1(Name, _Size)

#define Array_Extern(Type, Name) \
    extern Type* Name; \
    extern int _C1(Name, _Index); \
    extern int _C1(Name, _Size)

#define Array_Gtor(Type, Name) \
    Type* Name = (Type*)malloc(sizeof(Type) * 8); \
    int _C1(Name, _Index) = -1; \
    int _C1(Name, _Size)  = 8

#define Array_Ctor(Type, Name) \
    Name = (Type*)malloc(sizeof(Type) * 8); \
    _C1(Name, _Index) = -1; \
    _C1(Name, _Size)  = 8

//Omit unused warnings.
#define RFree(...) __RFree(__VA_ARGS__, (void*)(- 1))
void __RFree(void* a, ...);
#define Array_Dtor(Type, Name) \
    RFree(Name); \
    _C1(Name, _Size)  = _C1(Name, _Size); \
    _C1(Name, _Index) = _C1(Name, _Index)

#define Array_ObjDtor(Type, Name) \
    do{ \
        int Array_i; \
        for(Array_i = 0; Array_i <= _C1(Name, _Index); Array_i ++) \
            _C1(Type, _Dtor)(Name + Array_i); \
        _C1(Name, _Size) = _C1(Name, _Size); \
        _C1(Name, _Index) = _C1(Name, _Index); \
    }while(0)

//Resize means to allocate required capacity but do not change Array_Index.
#define Array_Resize(Type, Array, NewSize) \
    _ProtoArray_Resize(Type, Array, _C1(Array, _Index), _C1(Array, _Size), \
        NewSize)

#define Array_ObjResize(Type, Array, NewSize) \
    do{ \
        int Array_i; \
        if(NewSize < _C1(Array, _Index) + 1) \
        { \
            for(Array_i = _C1(Array, _Index); Array_i > NewSize; Array_i --) \
                _C1(Type, _Dtor)(Array + Array_i); \
            Array_Resize(Type, Array, NewSize); \
        }else if(NewSize > _C1(Array, _Index) + 1) \
        { \
            Array_Resize(Type, Array, NewSize); \
            for(Array_i = _C1(Array, _Index) + 1; Array_i < NewSize; \
                Array_i ++) \
                _C1(Type, _Ctor)(Array + Array_i); \
        } \
    }while(0)

#define Array_From(Type, DestArray, SorcArray) \
    do{ \
        int Array_i; \
        Array_Resize(Type, DestArray, _C1(SorcArray, _Index) + 1); \
        _C1(DestArray, _Index) = _C1(SorcArray, _Index); \
        for(Array_i = 0; Array_i <= _C1(SorcArray, _Index); Array_i ++) \
            DestArray[Array_i] = SorcArray[Array_i]; \
    }while(0)

#define Array_ObjFrom(Type, DestArray, SorcArray) \
    do{ \
        int Array_i; \
        Array_ObjResize(Type, DestArray, _C1(SorcArray, _Index) + 1); \
        _C1(DestArray, _Index) = _C1(SorcArray, _Index); \
        for(Array_i = 0; Array_i <= _C1(SorcArray, _Index); Array_i ++) \
            RCall(Type, From)(DestArray + Array_i, SorcArray + Array_i); \
    }while(0)

#define Array_Push(Type, Array, Data) \
    _ProtoArray_Push(Type, Array, _C1(Array, _Index), _C1(Array, _Size), Data)

#define Array_PushNull(Type, Array) \
    _ProtoArray_PushNull(Type, Array, _C1(Array, _Index), _C1(Array, _Size))

#define Array_PushObj(Type, Array, ObjRef) \
    _ProtoArray_PushNull(Type, Array, _C1(Array, _Index), _C1(Array, _Size)); \
    Type##_Ctor(& Array[_C1(Array, _Index)]);\
    Type##_Copy(& Array[_C1(Array, _Index)], ObjRef)

#define Array_Pop(Type, Array) \
    _ProtoArray_Pop(Type, Array, _C1(Array, _Index), _C1(Array, _Size))

#define Array_Insert(Type, Array, Index, Data) \
    _ProtoArray_Insert(Type, Array, _C1(Array, _Index), _C1(Array, _Size), \
        Index, Data)

#define Array_InsertNull(Type, Array, Index) \
    _ProtoArray_InsertNull(Type, Array, _C1(Array, _Index), _C1(Array, _Size), \
        Index)

#define Array_Remove(Type, Array, Index) \
    _ProtoArray_Remove(Type, Array, _C1(Array, _Index), _C1(Array, _Size), \
        Index)

#define Array_RemoveRange(Type, Array, LIndex, HIndex) \
    _ProtoArray_RemoveRange(Type, Array, _C1(Array, _Index), \
    _C1(Array, _Size), LIndex, HIndex)

//---------------------Advanced Macros--------------------
#define Array_IncFind(Dest, Type, Array, Data) \
    Array_IncFindFrom(Dest, Type, Array, Data, 0)

#define Array_IncFindFrom(Dest, Type, Array, Data, Index) \
    do \
    { \
        int Array_i; \
        for(Array_i = Index; Array_i <= _C1(Array, _Index); Array_i ++) \
            if((Array)[Array_i] > (Data)) \
                break; \
        Dest = Array_i; \
    } while(0)

#define Array_DecFind(Dest, Type, Array, Data) \
    Array_DecFindFrom(Dest, Type, Array, Data, 0)

#define Array_DecFindFrom(Dest, Type, Array, Data, Index) \
    do \
    { \
        int Array_i; \
        for(Array_i = Index; Array_i <= _C1(Array, _Index); Array_i ++) \
            if(Array[Array_i] < (Data)) \
                break; \
        Dest = Array_i; \
    } while(0)

//---------------------Auxillaries------------------------

#define TopOf(Array) Array[_C1(Array, _Index)]

#define Array_Par(Type, Array) Type* Array, int _C1(Array, _Index), \
    int _C1(Array, _Size)
#define Array_Arg(Type, Array) Array, _C1(Array, _Index), _C1(Array, _Size)

#endif //RUTIL2_ARRAY_H

