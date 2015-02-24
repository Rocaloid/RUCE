#include <stdio.h>
#include "Core/OO.h"
#include "Structure/PMatch.h"
#include "MyClass.h"
#include "MyPrint.h"

RClass(MyInherit)
{
    RInherit(MyClass);
    int Prop3;
};

RCtor(MyInherit)
{
    MyClass_Ctor(MyBase(MyClass));
    RInit(MyInherit);
    printf("MyInherit: %d\n", MyBase(RObject) -> ClassID);
}

RDtor(MyInherit)
{
    printf("MyInherit Dtor\n");
    MyClass_Dtor(MyBase(MyClass));
}

int MyInherit_Method(MyInherit* This)
{
    Array_Push(int, MyBase(MyClass) -> MyArray, 
        MyBase(MyClass) -> Prop1 + 
        MyBase(MyClass) -> Prop2 + This -> Prop3);
    return TopOf(MyBase(MyClass) -> MyArray);
}

RInterface_AddMethod(IObject_Method, MyInherit_Method, MyInherit);
int IObject_Method(void* This)
{
    RInterface_DefCall(IObject_Method, This);
    return 0;
}


int main(void)
{
    MyClass a;
    MyInherit b;
    RNew(MyClass, & a);
    RNew(MyInherit, & b);
    //MyClass_Ctor(& a);
    //MyInherit_Ctor(& b);
    a.Prop1 = 5;
    a.Prop2 = 3;
    b._Base.Prop1 = 8;
    b._Base.Prop2 = 10;
    b.Prop3 = 15;
    
    printf("MyClass_Method: %d\n", MyClass_Method((MyClass*)(& b)));
    printf("MyInherit_Method: %d\n", MyInherit_Method(& b));
    printf("IObject_Method of MyClass: %d\n", IObject_Method(& a));
    printf("IObject_Method of MyInherit: %d\n", IObject_Method(& b));
    
    PMatch_Double_Double c;
    RNew(PMatch_Double_Double, & c);
    //PMatch_Double_Double_Ctor(& c);
    PMatch_Double_Double_AddPair(& c, 0, 1.5452334545);
    PMatch_Double_Double_AddPair(& c, 1, 2.53245345);
    PMatch_Double_Double_AddPair(& c, 5, 0);
    PMatch_Double_Double_AddPair(& c, 6, 0.3454353545);
    int i;
    for(i = 0; i < 10; i ++)
    {
        printf("%d, %.9lf\n", i, PMatch_Double_Double_Query(& c, (double)i / 2.0).Y);
    }
    //PMatch_Double_Double_Dtor(& c);
    
    MyPrint_Float(213.234353423);
    MyPrint_Double(213.234353423);
    
    //MyClass_Dtor(& a);
    //MyInherit_Dtor(& b);
    RDelete(& a, & b, & c);
    return 0;
}

