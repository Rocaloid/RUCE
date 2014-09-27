#ifndef RUCE_RUDB_H
#define RUCE_RUDB_H

#include "RUCEData.h"

#define RUDB_VERSION 4

int RUCE_RUDB_Load(RUCE_DB_Entry* Dest, String* Path);
int RUCE_RUDB_Save(RUCE_DB_Entry* Sorc, String* Path);

#endif //RUCE_RUDB_H

