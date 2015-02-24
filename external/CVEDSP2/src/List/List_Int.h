#ifndef CDSP2_LIST_INT_H
#define CDSP2_LIST_INT_H

#define First CDSP2_List_Int* This

#include <RUtil2.h>

RClass(CDSP2_List_Int)
{
    RInherit(RObject);
    
    //Public
    Array_Define(int, Frames);
};

void CDSP2_List_Int_CtorSize(First, int Size);
void CDSP2_List_Int_Resize(First, int Size);
void CDSP2_List_Int_From(First, CDSP2_List_Int* Sorc);
void CDSP2_List_Int_FromArray(First, int* Sorc, int Sorc_Index);
void CDSP2_List_Int_Clear(First);

void CDSP2_List_Int_ToPMatch_Float(First, PMatch_Float_Float* Dest);
void CDSP2_List_Int_ToPMatch_Double(First, PMatch_Double_Double* Dest);

int  CDSP2_List_Int_Fetch(First, int Index);
int  CDSP2_List_Int_IndexBefore(First, int Position);
int  CDSP2_List_Int_IndexAfter(First, int Position);
int  CDSP2_List_Int_Extract(First, int Index);
int  CDSP2_List_Int_ExtractBefore(First, int Position);
int  CDSP2_List_Int_ExtractAfter(First, int Position);
void CDSP2_List_Int_Remove(First, int Index);
void CDSP2_List_Int_RemoveRange(First, int LIndex, int HIndex);
void CDSP2_List_Int_RemoveRangeP(First, int LPosition, int HPosition);

int  CDSP2_List_Int_Add(First, int Position);

#undef  First
#endif

