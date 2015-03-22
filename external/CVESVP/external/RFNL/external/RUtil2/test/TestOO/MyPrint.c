#include <stdio.h>
#include "MyPrint.h"
#include "Core/OO.h"

#if 0
#include "_MyPrint.rc"
#endif
#define _RTAddress "_MyPrint.rc"
#define _ClassName
#define _Attr 1

#define _T1 Float
#include "Core/RTemplate.h"

#define _T1 Double
#include "Core/RTemplate.h"

void Check_Float(float Sorc)
{
    printf("Float is %f\n", Sorc);
}

void Check_Double(double Sorc)
{
    printf("Double is %.9lf\n", Sorc);
}

