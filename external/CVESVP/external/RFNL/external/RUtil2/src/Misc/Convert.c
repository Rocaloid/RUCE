#include "Convert.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void CStrChars(String* Dest, char* Sorc)
{
    String_SetChars(Dest, Sorc);
}

void CStrBool(String* Dest, int Sorc)
{
    if(Sorc)
        String_SetChars(Dest, "true");
    else
        String_SetChars(Dest, "false");
}

void CStrChar(String* Dest, char Sorc)
{
    String_AllocLength(Dest, 1);
    Dest -> Data[0] = Sorc;
    Dest -> Data_Index = 0;
}

void CStrInt(String* Dest, int Sorc)
{
    char buffer[20];
    sprintf((char*)buffer, "%d", Sorc);
    String_SetChars(Dest, buffer);
}

void CStrFloat(String* Dest, float Sorc)
{
    char buffer[30];
    sprintf((char*)buffer, "%f", Sorc);
    String_SetChars(Dest, buffer);
}

void CStrDouble(String* Dest, double Sorc)
{
    char buffer[30];
    sprintf((char*)buffer, "%lf", Sorc);
    String_SetChars(Dest, buffer);
}

void CStrFloatF(String* Dest, float Sorc, char* Format)
{
    char buffer[30];
    sprintf((char*)buffer, Format, Sorc);
    String_SetChars(Dest, buffer);
}

void CStrDoubleF(String* Dest, double Sorc, char* Format)
{
    char buffer[30];
    sprintf((char*)buffer, Format, Sorc);
    String_SetChars(Dest, buffer);
}

int CBoolStr(String* Sorc)
{
    if(String_EqualChars(Sorc, "true"))
        return 1;
    return 0;
}

int CBoolChars(char* Sorc)
{
    if(! strcmp(Sorc, "true"))
        return 1;
    return 0;
}

int CVBBoolStr(String* Sorc)
{
    if(String_EqualChars(Sorc, "True"))
        return 1;
    return 0;
}

int CVBBoolChars(char* Sorc)
{
    if(! strcmp(Sorc, "True"))
        return 1;
    return 0;
}

char CCharStr(String* Sorc)
{
    return Sorc -> Data[0];
}

char CCharChars(char* Sorc)
{
    return Sorc[0];
}

short int CShortStr(String* Sorc)
{
    char* chars = String_GetChars(Sorc);
    int ret = atoi(chars);
    return (short int)ret;
}
short int CShortChars(char* Sorc)
{
    int ret = atoi(Sorc);
    return (short int)ret;
}

int CIntStr(String* Sorc)
{
    char* chars = String_GetChars(Sorc);
    int ret = atoi(chars);
    return ret;
}
int CIntChars(char* Sorc)
{
    int ret = atoi(Sorc);
    return ret;
}

unsigned int CUIntStr(String* Sorc)
{
    char* chars = String_GetChars(Sorc);
    return (unsigned int)atol(chars);
}
unsigned int CUIntChars(char* Sorc)
{
    return (unsigned int)atol(Sorc);
}

float CFloatChars(char* Sorc)
{
    return atof(Sorc);
}

float CFloatStr(String* Sorc)
{
    return CFloatChars(String_GetChars(Sorc));
}

double CDoubleStr(String* Sorc)
{
    char* chars = String_GetChars(Sorc);
    return atof(chars);
}
double CDoubleChars(char* Sorc)
{
    return atof(Sorc);
}

