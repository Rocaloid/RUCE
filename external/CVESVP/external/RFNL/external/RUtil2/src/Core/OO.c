#include "OO.h"
#include <stdarg.h>

int __ClassID__;
static int __FirstRun__ = 0;

Array_Define(__CDtorFunc, __AutoCtor);
Array_Define(__CDtorFunc, __AutoDtor);

//Test if all initializations in RUtil2 are called only once./
/*
void __attribute__ ((constructor)) __Test_Init()
{
    printf("Test %x\n", (int)__Test_Init);
}
*/

void __attribute__ ((constructor)) __AutoCDtor_Init()
{
    if(__ClassID__ == 0 && __FirstRun__ == 0)
    {
        Array_Ctor(__CDtorFunc, __AutoCtor);
        Array_Ctor(__CDtorFunc, __AutoDtor);
        Array_Resize(__CDtorFunc, __AutoCtor, 100);
        Array_Resize(__CDtorFunc, __AutoDtor, 100);
        __FirstRun__ = 1;
    }
}

void __attribute__ ((destructor)) __AutoCDtor_Exit()
{
    if(__FirstRun__ != 0)
    {
        Array_Dtor(__CDtorFunc, __AutoCtor);
        Array_Dtor(__CDtorFunc, __AutoDtor);
        __FirstRun__ = 0;
    }
}

void __RNew(__CDtorFunc Ctor, ...)
{
    RObject* Obj;
    va_list Args;
    va_start(Args, Ctor);
    
    while(1)
    {
        Obj = va_arg(Args, void*);
        //End
        if(Obj == (RObject *)-1)
            break;
        Ctor(Obj);
    }
    
    va_end(Args);
}

void __RDelete(void* a, ...)
{
    RObject* Obj;
    __CDtorFunc Dtor;
    va_list Args;
    va_start(Args, a);
    
    //First
    Obj = a;
    Dtor = __AutoDtor[Obj -> ClassID];
    Dtor(Obj);
    while(1)
    {
        Obj = va_arg(Args, void*);
        //End
        if(Obj == (RObject *)-1)
            break;
        Dtor = __AutoDtor[Obj -> ClassID];
        Dtor(Obj);
    }
    
    va_end(Args);
}

