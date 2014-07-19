#define WITH_ASSERT_ABORT

#include <RUtil2.h>
#include <IO.h>

int main()
{
    
    String_FromChars(a, "a");
    String_FromChars(b, "./");
    String_FromChars(c, "./roto.json");
    RUCE_DB_Entry t, r;
    RUCE_DB_Entry_Ctor(& t);
    RUCE_DB_Entry_Ctor(& r);
    
    printf("TestLoad...\n");
    
    RAssert(RUCE_DB_LoadEntry(& t, & a, & b, & c) == 1, "Cannot Load Entry!");
    RUCE_DB_PrintEntry(& t);
    
    String_SetChars(& a, "b");
    
    printf("\nTestWrite...\n");
    
    RAssert(RUCE_DB_RUDBWriteEntry(& t, & a, & b) == 1, "Cannot Write RUDB Entry!");
    RAssert(RUCE_DB_RotoWriteEntry(& t, & a, & c) == 1, "Cannot Write Roto Entry!");
    RAssert(RUCE_DB_WaveWriteEntry(& t, & a, & b) == 1, "Cannot Write Wave Entry!");
    
    RAssert(RUCE_DB_LoadEntry(& r, & a, & b, & c) == 1, "Cannot Load Entry which have be written!");

    RUCE_DB_PrintEntry(& r);
    
    String_Dtor(& c);
    String_Dtor(& b);
    String_Dtor(& a);
    RUCE_DB_Entry_Dtor(& r);
    RUCE_DB_Entry_Dtor(& t);
    return 0;
}

#undef WITH_ASSERT_ABORT
