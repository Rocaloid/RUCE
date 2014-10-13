#ifndef RUCE_SOUNDBANK_H
#define RUCE_SOUNDBANK_H

/*
    This is an abstraction of RUCEData.h that hides RUDB and PitchModel.json
    stuffs from the engine level.
    Notice _RUCE_Soundbank is named to prevent collision with the definition in
    libRUCE(RUCE_Soundbank* as void*).
*/

#include <CVESVP.h>
#include "../external/cJSON/cJSON.h"
#include "RUCEData.h"
#include "libRUCE.h"

typedef struct
{
    String Directory;
    cJSON* PMEntries;
    cJSON* PMRoot;
    CSVP_PitchModel PMDefault;
} _RUCE_Soundbank;

int RUCE_SoundbankLoadEntry(RUCE_DB_Entry* Dest, RUCE_Soundbank* Bank,
    String* Name);
int RUCE_SoundbankLoadPitchModel(CSVP_PitchModel* Dest, RUCE_Soundbank* Bank,
    String* Name);

#endif

