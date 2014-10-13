#define WITH_ASSERT_ABORT

#include <RUtil2.h>
#include <RUCEData.h>

int main()
{
    String_FromChars(a, "./a.rudb");
    RUCE_DB_Entry t, r;
    RUCE_DB_Entry_Ctor(& t);
    RUCE_DB_Entry_Ctor(& r);
    
    printf("TestLoad...\n");
    
    int RUDBRet = RUCE_RUDB_Load(& t, & a);
    printf("RUDB_RUDB_Load returned %d.\n", RUDBRet);
    RAssert(RUDBRet == 1, "Cannot Load Entry!\n");
    RUCE_RUDB_Print(& t);
    
    String_SetChars(& a, "./b.rudb");
    
    printf("TestWrite...\n");
    
    RAssert(RUCE_RUDB_Save(& t, & a) == 1, "Cannot Write RUDB Entry!");
    
    RAssert(RUCE_RUDB_Load(& r, & a) == 1, "Cannot Load Entry which have"
        "been written!");
    
    RUCE_RUDB_Print(& r);
    
    String_Dtor(& a);
    RUCE_DB_Entry_Dtor(& r);
    RUCE_DB_Entry_Dtor(& t);
    return 0;
}

#undef WITH_ASSERT_ABORT

