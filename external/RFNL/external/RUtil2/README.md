RUtil2
======

Rocaloid Coding Utility 2

* Language: C99

* License: MIT License

Requirements
===
* CMake 2.8.4 or higher

Tutorial
===

####BUILD
    mkdir build
    cd build
    cmake ../RUtil2
    make
    sudo make install


####Hello World

    #include <RUtil2.h>
    int main()
    {
        String Hello;
        String_Ctor(& Hello);
        String_SetChars(& Hello, "Hello, world!\n");
        printf("%s", String_GetChars(& Hello));
        String_Dtor(& Hello);
        return 0;
    }

Compile:

    gcc hello.c -lRUtil2

####Join some Strings

    #include <RUtil2.h>
    int main()
    {
        String Str1, Str2;
        String_Ctor(& Str1);
        String_Ctor(& Str2);
    
        String_SetChars(& Str1, "Hello");
        String_SetChars(& Str2, "World");
        String_JoinChars(& Str1, ", ");
        String_Join(& Str1, & Str2);
        String_JoinChars(& Str1, "!\n");
        printf("%s", String_GetChars(& Str1));
    
        String_Dtor(& Str1);
        String_Dtor(& Str2);
        return 0;
    }

####Batch Constructor and Destructor

    #include <RUtil2.h>
    int main()
    {
        String Str1, Str2;
        RNew(String, & Str1, & Str2);

        String_SetChars(& Str1, "Hello");
        String_SetChars(& Str2, "World");
        String_JoinChars(& Str1, ", ");
        String_Join(& Str1, & Str2);
        String_JoinChars(& Str1, "!\n");
        printf("%s", String_GetChars(& Str1));
    
        RDelete(& Str1, & Str2);
        return 0;
    }

####Define a Class

<pre>
RClass(MyClass)
{
    //This must be placed on the top.
    //And make sure every class inherits RObject directly or indirectly.
    RInherit(RObject);

    int MyVal;
    String MyString;
};
</pre>

####Define the Constructor and Destructor

<pre>
RCtor(MyClass)
{
    String_Ctor(& This -> MyString);
    //Important!
    //This assigns a ClassID to the newly created instance of MyClass.
    //Each class in RUtil has a unique ClassID;
    //Each instance of the same class shares the same ClassID.
    RInit(MyClass);
}

RDtor(MyClass)
{
    String_Dtor(& This -> MyString);
}
</pre>

####Define some Methods!

<pre>
void MyClass_Set(MyClass* This, int SorcNum, String* SorcStr)
{
    This -> MyVal = SorcNum;
    String_Copy(& This -> MyString, SorcStr);
}

void MyClass_Print(MyClass* This)
{
    printf("MyVal is: %d\n", This -> MyVal);
    printf("MyString is : %s\n", String_GetChars(& This -> MyString));
}
</pre>

####The Complete Code & Use our Class

MyClass.h:

    #include <RUtil2.h>
    
    RClass(MyClass)
    {
        RInherit(RObject);
    
        int MyVal;
        String MyString;
    };

MyClass.c:

<pre>
#include "MyClass.h"

RCtor(MyClass)
{
    String_Ctor(& This -> MyString);
    RInit(MyClass);
}

RDtor(MyClass)
{
    String_Dtor(& This -> MyString);
}

void MyClass_Set(MyClass* This, int SorcNum, String* SorcStr)
{
    This -> MyVal = SorcNum;
    String_Copy(& This -> MyString, SorcStr);
}

void MyClass_Print(MyClass* This)
{
    printf("MyVal is: %d\n", This -> MyVal);
    printf("MyString is : %s\n", String_GetChars(& This -> MyString));
}
</pre>

main.c:

<pre>
#include "MyClass.h"

int main()
{
    MyClass MyInstance;
    MyClass_Ctor(& MyInstance);

    String Str;
    String_Ctor(& Str);
    String_SetChars(& Str, "abcdef");

    MyClass_Set(& MyInstance, 100, & Str);
    MyClass_Print(& MyInstance);

    RDelete(& MyInstance, & Str);
    return 0;
}
</pre>

Compile:

    gcc MyClass.c main.c -lRUtil2

####Define an Array

    #include <RUtil2.h>

    int main()
    {
        Array_Define(int, MyArray);
        Array_Ctor(int, MyArray);
        
        /* Some codes... */
        
        Array_Dtor(int, MyArray);
        return 0;
    }

####Access & Push elements to an Array

    #include <RUtil2.h>

    int main()
    {
        Array_Define(int, MyArray);
        Array_Ctor(int, MyArray);
        
        Array_Push(int, MyArray, 123);
        Array_Push(int, MyArray, 456);
        Array_Push(int, MyArray, 789);
        Array_Push(int, MyArray, 999);
        
        printf("%d %d\n", MyArray[0], MyArray[2]);
        
        Array_Dtor(int, MyArray);
        return 0;
    }

####Traverse an Array

    #include <RUtil2.h>

    int main()
    {
        Array_Define(int, MyArray);
        Array_Ctor(int, MyArray);
        
        Array_Push(int, MyArray, 123);
        Array_Push(int, MyArray, 456);
        Array_Push(int, MyArray, 789);
        Array_Push(int, MyArray, 999);
        
        int i;
        //ArrayName_Index is the index of the toppest element in the array.
        for(i = 0; i <= MyArray_Index; i ++)
            printf("%d\n", MyArray[i]);
        
        Array_Dtor(int, MyArray);
        return 0;
    }

####Manually Modify an Array

    #include <RUtil2.h>

    int main()
    {
        Array_Define(int, MyArray);
        Array_Ctor(int, MyArray);
        Array_Resize(int, MyArray, 4);
        
        MyArray[0] = 123;
        MyArray[1] = 456;
        MyArray[2] = 789;
        MyArray[3] = 999;
        MyArray_Index = 3;
        
        int i;
        for(i = 0; i <= MyArray_Index; i ++)
            printf("%d\n", MyArray[i]);
        
        Array_Dtor(int, MyArray);
        return 0;
    }

####Remove an Element

    #include <RUtil2.h>

    int main()
    {
        Array_Define(int, MyArray);
        Array_Ctor(int, MyArray);
        Array_Resize(int, MyArray, 4);
        
        MyArray[0] = 123;
        MyArray[1] = 456;
        MyArray[2] = 789;
        MyArray[3] = 999;
        MyArray_Index = 3;
        
        Array_Remove(int, MyArray, 2);
        
        int i;
        for(i = 0; i <= MyArray_Index; i ++)
            printf("%d\n", MyArray[i]);
        
        Array_Dtor(int, MyArray);
        return 0;
    }

####Insert an Element

    Array_Insert(int, MyArray, 2, 333);

####Array of Objects

    #include "MyClass.h"

    int main()
    {
        Array_Define(MyClass, MyArray);
        Array_Ctor(MyClass, MyArray);
        
        String Str;
        String_Ctor(& Str);
        String_SetChars(& Str, "asdf");
        
        Array_PushNull(MyClass, MyArray);
        MyClass_Ctor(& MyArray[MyArray_Index]);
        MyClass_Set(& MyArray[MyArray_Index], 123, & Str);
        Array_PushNull(MyClass, MyArray);
        MyClass_Ctor(& MyArray[MyArray_Index]);
        MyClass_Set(& MyArray[MyArray_Index], 456, & Str);
        /* ... */
        
        int i;
        for(i = 0; i <= MyArray_Index; i ++)
            MyClass_Print(& MyArray[i]);
        
        for(i = 0; i <= MyArray_Index; i ++)
            MyClass_Dtor(& MyArray[i]);
        Array_Dtor(MyClass, MyArray);
        RDelete(& Str);
        return 0;
    }

Syntactic sugar:

        for(i = 0; i <= MyArray_Index; i ++)
            MyClass_Dtor(& MyArray[i]);

is equivalent to and can be replaced by

        Array_ObjDtor(MyClass, MyArray);

and DO NOT forget to

        Array_Dtor(MyClass, MyArray);

**Notice: `Array_Resize` does not offer such automatic destructor call. You have to manually destruct the objects before using `Array_Resize` on an array of objects.**
