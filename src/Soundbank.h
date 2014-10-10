#ifndef RUCE_SOUNDBANK_H
#define RUCE_SOUNDBANK_H

#include <CVESVP.h>
#include "../external/cJSON/cJSON.h"

typedef struct
{
    String Directory;
    cJSON* PMEntries;
    cJSON* PMRoot;
    CSVP_PitchModel PMDefault;
} RUCE_Soundbank;

#endif

