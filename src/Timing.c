#include "Timing.h"
#include "Verbose.h"

Segmentation MatchUnitToNote(PMatch* Dest, RUCE_DB_Entry* SorcDB,
    RUCE_Session* Session, int NoteIndex)
{
    Verbose(4, "(function entrance)\n");
    Segmentation Ret;
    int SampleRate = Session -> SampleRate;
    int SorcHead = SorcDB -> FrameList[0].Position;
    RUCE_Note* Note = & Session -> NoteList[NoteIndex];
    
    double ST0 = Sample2Sec(SorcHead);
    double STV = SorcDB -> VOT;
    double DTV = Note -> CParamSet.DurConsonant <= 0 ? SorcDB -> VOT :
        Note -> CParamSet.DurConsonant;
    double STD = Sample2Sec(TopOf(SorcDB -> FrameList).Position);
    double DTD = Note -> Duration + Note -> CParamSet.DeltaDuration + DTV;
    
    //ST0 -> DT0, STV -> DTV, STD -> DTD
    RCall(PMatch, AddPair)(Dest, 0, 0);
    RCall(PMatch, AddPair)(Dest, DTV, STV);
    RCall(PMatch, AddPair)(Dest, DTD, STD);
    
    //ST1 -> DT1, ST2 -> DT2
    double SP1 = SorcDB -> InvarLeft - STV;
    double SP2 = SorcDB -> InvarRight - STV;
    double DP1, DP2;
    
    if(Note -> CParamSet.DurInitial > 0)
        DP1 = Note -> CParamSet.DurInitial;
    if(Note -> CParamSet.DurFinal > 0)
        DP2 = STD - Note -> CParamSet.DurFinal - STV;
    
    //Final not determined yet.
    if(Note -> CParamSet.DurInitial > 0 && Note -> CParamSet.DurFinal <= 0)
    {
        DP2 = DTD - (STD - SP2);
        //Prevent over-long finals.
        if(DP2 - DP1 < DTD - DP2)
            DP2 = DP1 + (DTD - DP1) / 2.0;
    }
    
    //Initial not determined yet.
    if(Note -> CParamSet.DurFinal > 0 && Note -> CParamSet.DurInitial <= 0)
    {
        DP1 = SP1;
        //Prevent over-long initials.
        if(DP1 > DP2 - DP1)
            DP1 = DP2 / 2;
    }
    
    //Both final & initial not determined yet.
    if(Note -> CParamSet.DurFinal <= 0 && Note -> CParamSet.DurInitial <= 0)
    {
        DP1 = SP1;
        DP2 = DTD - (STD - SP2);
        
        if(DP2 < DP1)
        {
            DP1 = DP2 = SP1 / (SP1 + SP2) * DTD;
            DP1 *= 0.7;
            DP2 = DTD - (DTD - DP2) * 0.7;
        }
    }
    
    RCall(PMatch, AddPair)(Dest, DTV + DP1, STV + SP1);
    RCall(PMatch, AddPair)(Dest, DTV + DP2, STV + SP2);
    
    PMatch RevTimeMatch;
    RCall(PMatch, Ctor)(& RevTimeMatch);
    RCall(PMatch, InvertTo)(Dest, & RevTimeMatch);
    Ret.T0 = RCall(PMatch, Query)(& RevTimeMatch, ST0).Y;
    Ret.TV = DTV;
    Ret.T1 = DTV + DP1;
    Ret.T2 = DTV + DP2;
    Ret.TD = DTD;
    
    RCall(PMatch, Dtor)(& RevTimeMatch);
    
    return Ret;
}

