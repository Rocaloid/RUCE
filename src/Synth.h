#ifndef RUCE_SYNTH_H
#define RUCE_SYNTH_H

#include "GetPara.h"
#include "IO.h"
#include <CVESVP.h>
#include <RUtil2.h>
#include "Common.h"

int RUCE_SynthUnit(_Wave* Dest, _Wave* Sorc, RUCE_DB_Entry* SorcDB,
    CSVP_PitchModel* PM, RUCE_ResamplerPara* Para);

#endif

