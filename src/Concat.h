#ifndef RUCE_CONCAT_H
#define RUCE_CONCAT_H

#include <CVEDSP2.h>
#include <Common.h>

int RUCE_Concat_UnvoicedFadeOut(Wave* Dest, int Align, int Length);
int RUCE_Concat_NoiseFadeIn(Wave* Dest, int Align, int Length);

#endif

