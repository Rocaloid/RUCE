#include "Synth.h"

RCtor(RUCE_UnitParam)
{
    String_Ctor(& This -> Input);
    String_Ctor(& This -> Output);
    PMatch_Float_Float_Ctor(& This -> Freq);
    
    This -> InvarLeft   = 0;
    This -> InvarRight  = 0;
    This -> LenRequire  = -1;
    This -> FixedLength = 0;
    This -> Velocity    = 0;
    This -> Volume      = 100;
    This -> Modulation  = 0;
    
    This -> FlagPara.Breathness    = 50;
    This -> FlagPara.Gender        = 0;
    This -> FlagPara.DeltaDuration = 0;
    This -> FlagPara.PhaseSync     = 1;
    This -> FlagPara.Verbose       = 0;
    This -> FlagPara.CStretch      = 0;
    This -> FlagPara.COffset       = 0;
    This -> FlagPara.DeltaSeg1     = 0;
    This -> FlagPara.DeltaSeg2     = 0;
    This -> FlagPara.SmoothenRate  = 0.6;
    This -> FlagPara.SmoothenRadius = 0.05;
    
    RInit(RUCE_UnitParam);
}

RDtor(RUCE_UnitParam)
{
    String_Dtor(& This -> Input);
    String_Dtor(& This -> Output);
    PMatch_Float_Float_Dtor(& This -> Freq);
}

