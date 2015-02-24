#include <RUtil2.h>

//When using RFNL you have to call this function to ensure libRFNL ls linked to
//  your program. Otherwise programs which calls only inline functions in RFNL
//  may crush(because constructors are not called).
void RFNL_Use()
{
    //Empty
}

//Never call this function.
//This function is used to ensure that RUtil2 is completely linked into RFNL.
void __RFNL_RUtil2_Link()
{
    RDelete((void*)0);                          //OO
    RAlloc(0);                                  //RAlloc
    File_Ctor((void*)0);                        //File
    WaveFile_Ctor((void*)0);                    //WaveFile
    CStrChars((void*)0, (void*)0);              //Convert
    Sort_Inc_Float((void*)0, (void*)0, 0);      //Sort
    PMatch_Float_Float_Ctor((void*)0);          //PMatch
    String_Ctor((void*)0);                      //String
    Base64_Encode((void*)0, (void*)0, 0);       //Base64
    Tune_SetPitchNameMode(0);                   //Tune
    CRC32Sum((void*)0, 0, 0);                   //CRC32
}

