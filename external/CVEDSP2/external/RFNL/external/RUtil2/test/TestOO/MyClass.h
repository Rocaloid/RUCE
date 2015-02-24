#ifndef MYCLASS_H
#define MYCLASS_H

#include "Core/OO.h"
#include "Structure/Array.h"

RClass(MyClass)
{
    RInherit(RObject);
    int Prop1;
    int Prop2;
    Array_Define(int, MyArray);
};

RInterface_Define(int, IObject_Method, void*);

int MyClass_Method(MyClass* This);

#endif

