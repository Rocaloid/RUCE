#define WITH_ASSERT_ABORT

#include <RUtil2.h>
#include <RUCEData.h>

int main()
{
    
    String_FromChars(a, "a");
    String_FromChars(b, "./");
    RUCE_DB_Entry t, r;
    RUCE_DB_Entry_Ctor(& t);
    RUCE_DB_Entry_Ctor(& r);
    
    printf("TestLoad...\n");
    
    int RUDBRet = RUCE_DB_LoadEntry(& t, & a, & b);
    RAssert(RUCE_DB_LoadExternWave(&t, & a, & b) == 1);
    printf("RUDB_LoadDB returned %d.\n", RUDBRet);
    RAssert(RUDBRet == 1, "Cannot Load Entry!");
    RUCE_DB_PrintEntry(& t);
    
    String_SetChars(& a, "b");
    
    printf("\nTestWrite...\n");
    
    RAssert(RUCE_DB_RUDBWriteEntry(& t, & a, & b) == 1, "Cannot Write RUDB Entry!");
    
    RAssert(RUCE_DB_LoadEntry(& r, & a, & b) == 1, "Cannot Load Entry which have be written!");

    RUCE_DB_PrintEntry(& r);
    
    String_Dtor(& b);
    String_Dtor(& a);
    RUCE_DB_Entry_Dtor(& r);
    RUCE_DB_Entry_Dtor(& t);
    return 0;
}

#undef WITH_ASSERT_ABORT

