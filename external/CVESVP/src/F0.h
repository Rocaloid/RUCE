#ifndef CSVP_F0_H
#define CSVP_F0_H

#include <CVEDSP2.h>

#define CSVP_F0_SpecStep 0
#define CSVP_F0_YIN 1

typedef struct
{
    float HFreq;
    float LFreq;
    float Variance;
    float Threshold;
    
    char Adlib;
    char Method;
    
    struct
    {
        int W;
        float Threshold;
    } YIN;
} CSVP_F0Option;

#include <RUtil2.h>

#if 0
#include "_F0.h"
#include "Iterator/_F0Iterlyzer.h"
#endif

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/_F0.h"
#else
#define _RTAddress "_F0.h"
#endif

#define _ClassName _
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

//-----
#include <RUtil2.h>

#ifdef __CSVP_Install
#define _RTAddress "CVESVP/Iterator/_F0Iterlyzer.h"
#else
#define _RTAddress "Iterator/_F0Iterlyzer.h"
#endif

#define _ClassName CSVP_F0Iterlyzer_
#define _Attr 1

#define _T1 Float
#include <RUtil2/Core/RTemplate.h>

#define _T1 Double
#include <RUtil2/Core/RTemplate.h>

#endif

