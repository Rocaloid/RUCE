#ifndef RUCE_PITCHBEND_H
#define RUCE_PITCHBEND_H

#include <RUtil2.h>

int RUCE_Pitchbend_GetLength(String* Sorc);
void RUCE_Pitchbend_Decode(short* Dest, String* Sorc);

#endif //RUCE_PITCHBEND_H
