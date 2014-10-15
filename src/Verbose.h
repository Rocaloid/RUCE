#ifndef RUCE_VERBOSE_H
#define RUCE_VERBOSE_H

#include "libRUCE.h"

extern int _RUCE_VerboseLevel;
#define Verbose(Level, ...) \
    if(_RUCE_VerboseLevel >= Level) fprintf(stderr, __VA_ARGS__)

#endif

