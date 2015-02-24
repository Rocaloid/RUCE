#ifndef RUTIL2_CONVERT_H
#define RUTIL2_CONVERT_H
#include "../Structure/String.h"

//Wrapper for template definition.
#define C_String_Chars CStrChars
#define C_String_Bool CStrBool
#define C_String_Char CStrChar
#define C_String_Int CStrInt
#define C_String_Float CStrFloat
#define C_String_Double CStrDouble
#define C_String_FloatF CStrFloatF
#define C_String_DoubleF CStrDoubleF

#define C_Bool_String CBoolStr
#define C_Bool_StringVB CVBBoolStr
#define C_Bool_Chars CBoolChars
#define C_Bool_CharsVB CVBBoolChars

#define C_Char_String CCharStr
#define C_Char_Chars CCharChars

#define C_Short_String CShortStr
#define C_Short_Chars CShortChars

#define C_Int_String CIntStr
#define C_Int_Chars CIntChars

#define C_UInt_String CUIntStr
#define C_UInt_Chars CUIntChars

#define C_Float_String CFloatStr
#define C_Float_Chars CFloatChars

#define C_Double_String CDoubleStr
#define C_Double_Chars CDoubleChars

void CStrChars(String* Dest, char* Sorc);
void CStrBool(String* Dest, int Sorc);
void CStrChar(String* Dest, char Sorc);
void CStrInt(String* Dest, int Sorc);
void CStrFloat(String* Dest, float Sorc);
void CStrDouble(String* Dest, double Sorc);
void CStrFloatF(String* Dest, float Sorc, char* Format);
void CStrDoubleF(String* Dest, double Sorc, char* Format);

int CBoolStr(String* Sorc);
int CVBBoolStr(String* Sorc);
int CBoolChars(char* Sorc);
int CVBBoolChars(char* Sorc);

char CCharStr(String* Sorc);
char CCharChars(char* Sorc);

short int CShortStr(String* Sorc);
short int CShortChars(char* Sorc);

int CIntStr(String* Sorc);
int CIntChars(char* Sorc);

unsigned int CUIntStr(String* Sorc);
unsigned int CUIntChars(char* Sorc);

float CFloatStr(String* Sorc);
float CFloatChars(char* Sorc);

double CDoubleStr(String* Sorc);
double CDoubleChars(char* Sorc);

#endif //RUTIL2_CONVERT_H

