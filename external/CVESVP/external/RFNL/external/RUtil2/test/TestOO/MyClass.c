#include "MyClass.h"
#include <stdio.h>

RCtor(MyClass)
{
    This -> Prop1 = 0;
    This -> Prop2 = 0;
    Array_Ctor(int, This -> MyArray);
    RInit(MyClass);
    printf("MyClass: %d\n", MyBase(RObject) -> ClassID);
}

RDtor(MyClass)
{
    printf("MyClass Dtor\n");
    int i;
    for(i = 0; i <= MyBase(MyClass) -> MyArray_Index; i ++)
    {
        printf("MyArray[%d] = %d\n", i, MyBase(MyClass) -> MyArray[i]);
    }
    Array_Dtor(int, This -> MyArray);
}

int MyClass_Method(MyClass* This)
{
    Array_Push(int, This -> MyArray, This -> Prop1 + This -> Prop2);
    return TopOf(This -> MyArray);
}

RInterface_Make(IObject_Method);
RInterface_AddMethod(IObject_Method, MyClass_Method, MyClass);

