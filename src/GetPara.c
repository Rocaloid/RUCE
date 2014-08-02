#include "GetPara.h"
#include "Pitchbend.h"
#include <stdio.h>
#include <RUtil2.h>

void PrintUsage()
{
    printf("Usage: RUCE_CLI <input file> <output file> <pitch percent>\n"
           "                <velocity> [<flags> [<offset> <length require>\n"
           "                [<fixed length> [<end blank> [<volume>\n"
           "                [<modulation> [<pitch bend>...]]]]]]]\n");
}

RCtor(RUCE_ResamplerPara)
{
    String_Ctor(& This -> Input);
    String_Ctor(& This -> Output);
    PMatch_Float_Float_Ctor(& This -> Freq);
    
    This -> InvarLeft   = 0;
    This -> InvarRight  = 0;
    This -> LenRequire  = 0;
    This -> FixedLength = 0;
    This -> Velocity    = 0;
    This -> Volume      = 0;
    This -> Modulation  = 0;
    
    This -> FlagPara.Breathness    = 50;
    This -> FlagPara.Gender        = 0;
    This -> FlagPara.DeltaDuration = 0;
    This -> FlagPara.PhaseSync     = 1;
    This -> FlagPara.Verbose       = 0;
    
    RInit(RUCE_ResamplerPara);
}

RDtor(RUCE_ResamplerPara)
{
    String_Dtor(& This -> Input);
    String_Dtor(& This -> Output);
    PMatch_Float_Float_Dtor(& This -> Freq);
}

int RUCE_ParsePara(RUCE_ResamplerPara* Dest, int argc, char** argv)
{
    int i;
    int Ret = 1;
    int CLV = argc;
    int EnablePitchConv = 1;
    
    String PP, PBD;
    RNew(String, & PP, & PBD);
    switch(CLV)
    {
        case 14:
            String_SetChars(& PP, argv[3]);
            String_SetChars(& PBD, argv[13]);
            float Tempo = atof(argv[12] + 1);
            if(Tempo <= 0)
            {
                fprintf(stderr, "[Error] Invalid tempo as '%s'.\n", argv[12]);
                Ret = 0;
                break;
            }
            int DataNum = RUCE_Pitchbend_GetLength(& PBD);
            short* Data = RAlloc(DataNum * sizeof(short));
            float Freq = Tune_SPNToFreq_Float(& PP);
            RUCE_Pitchbend_Decode(Data, & PBD);
            for(i = 0; i < DataNum; ++ i)
            {
                Data[i] = Data[i] > 2048 ? Data[i] - 4096 : Data[i];
                PMatch_Float_Float_AddPair(& Dest -> Freq, 
                    Tune_BeatToTime_Float(Tempo, (((float)i) / 96.0f)),
                    Tune_AddCentToFreq_Float(Freq, Data[i]));
            }
            RFree(Data);
            EnablePitchConv = 0; // Disable standalone pitch conv.
            CLV -= 2;
        
        case 13:
        case 12:
            Dest -> Modulation = atof(argv[11]);
            -- CLV;
            
        case 11:
            Dest -> Volume = atof(argv[10]);
            if(Dest -> Volume < 0.0f)
            {
                Ret = 0;
                fprintf(stderr, "[Error] Invalid volume as '%s'.\n", argv[10]);
                break;
            }
            -- CLV;
            
        case 10:
            Dest -> InvarRight = atof(argv[9]) / 1000.0f;
            if(Dest -> InvarRight < 0.0f)
            {
                Ret = 0;
                fprintf(stderr, "[Error] Invalid end bank as '%s'.\n", argv[9]);
                break;
            }
            -- CLV;
            
        case 9:
            Dest -> FixedLength = atof(argv[8]) / 1000.0f;
            if(Dest -> FixedLength <= 0.0f)
            {
                Ret = 0;
                fprintf(stderr, "[Error] Invalid fixed length as '%s'.\n",
                    argv[8]);
                break;
            }
            -- CLV;
            
        case 8:
            Dest -> LenRequire = atof(argv[7]) / 1000.0f;
            if(Dest -> LenRequire < 0.0f)
            {
                Ret = 0;
                fprintf(stderr, "[Error] Invalid length require as '%s'.\n",
                    argv[7]);
                break;
            }
            Dest -> InvarLeft = atof(argv[6]) / 1000.0f;
            if(Dest -> InvarLeft < 0.0f)
            {
                Ret = 0;
                fprintf(stderr, "[Error] Invalid offset as '%s'.\n", argv[6]);
                break;
            }
            CLV -= 2;
            
        case 6:;
            char* CFlags = argv[5];
            int FlagLen = strlen(CFlags);
            i = 0;
            
            #define IsLetter(x) (((x) >= 'a' && (x) <= 'z') || \
                                 ((x) >= 'A' && (x) <= 'Z'))
            //Parse flags
            while(i < FlagLen)
            {
                float Value = atof(CFlags + i + 1);
                switch(CFlags[i])
                {
                    case 'B':
                        //Avoid floating point error due to extremely small
                        //  number in log scale converison.
                        if(Value < 0.1)
                            Value = 0.1;
                        
                        Dest -> FlagPara.Breathness    = Value;
                        if(Value < 0)
                            fprintf(stderr, "[Warning] Invalid breathness "
                                "parameter.\n");
                    break;
                    case 'g':
                        Dest -> FlagPara.Gender        = Value;
                        if(Value <= 0)
                            fprintf(stderr, "[Warning] Invalid gender parameter"
                                ".\n");
                    break;
                    case 'd':
                        Dest -> FlagPara.DeltaDuration = Value;
                    break;
                    case 'p':
                        Dest -> FlagPara.PhaseSync     = Value;
                        if(Value < 0)
                            fprintf(stderr, "[Warning] Invalid phase "
                                "synchronicity.\n");
                    break;
                    case 'V':
                        Dest -> FlagPara.Verbose       = 1;
                    break;
                    default:
                        fprintf(stderr, "[Warning] Unrecognized flag '%c' with "
                            "parameter %f.\n", CFlags[i], Value);
                }
                
                i ++;
                while(i < FlagLen && ! IsLetter(CFlags[i])) i ++;
            }
            -- CLV;
            
        case 5:
            Dest -> Velocity = atof(argv[4]);
            if(Dest -> Velocity < 0.0f)
            {
                Ret = 0;
                fprintf(stderr, "[Error] Invalid velocity as '%s'.\n", argv[4]);
                break;
            }
            if(EnablePitchConv)
            {
                String_SetChars(& PP, argv[3]);
                PMatch_Float_Float_AddPair(& Dest -> Freq, 0, 
                    Tune_SPNToFreq_Float(& PP));
            }
            String_SetChars(& (Dest -> Input), argv[1]);
            String_SetChars(& (Dest -> Output), argv[2]);
            break;
            
        default:
            PrintUsage();
            Ret = -1;
    };
    
    RDelete(& PP, & PBD);
    return Ret;
}

