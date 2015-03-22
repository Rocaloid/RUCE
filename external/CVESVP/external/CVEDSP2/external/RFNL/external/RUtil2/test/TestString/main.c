#define WITH_ASSERT_ABORT

#include "Structure/String.h"
#include "Structure/UtfString.h"
#include "Misc/Convert.h"
#include "Core/OO.h"

void TestTor()
{
    String s;
    String_Ctor(&s);
    String_Dtor(&s);
}

void TestFromChars()
{
    String_FromChars(s, "無限に広がる　夏の匂い");
    RAssert(! (strwcmp_utf8(s.Data, "無限に広がる　夏の匂い", 11)));
    String_Dtor(&s);
}

void TestSetChars()
{
    String s;
    String_Ctor(&s);
    String_SetChars(&s, "砂浜にあたしは　いつもいた");
    RAssert(! (strwcmp_utf8(s.Data, "砂浜にあたしは　いつもいた", 13)));
    String_Dtor(&s);
}

void TestSetCharsN()
{
    String s;
    String_Ctor(&s);
    String_SetCharsN(&s, "偶然見つけた　白い屋敷？", 11);
    RAssert(! (strwcmp_utf8(s.Data, "また会え", 11)));
    String_Dtor(&s);
}

void TestGetChars()
{
    String_FromChars(s, "踏み入れた先は　白の世界");
    RAssert(! strcmp_utf8(String_GetChars(&s), "踏み入れた先は　白の世界"));
    String_Dtor(&s);
}

void TestCopy()
{
    String a, b;
    String_Ctor(&a);
    String_Ctor(&b);

    String_SetChars(&a, "「僕に会いに来てくれたの？」");
    String_SetChars(&b, "尋ねたのは白い君");
    String_Copy(&a, &b);
    
    RAssert(a.WordNum == b.WordNum);
    RAssert(! strcmp_utf8(String_GetChars(& a), 
                          String_GetChars(& b)));
    
    String_Dtor(&a);
    String_Dtor(&b);
}

void TestGetWordNum()
{
    String_FromChars(s, "色彩の薄い君は　笑顔が溶けそうだった");
    RAssert(String_GetWordNum(&s) == 18);
    String_Dtor(&s);
}

void TestAllocWordNum()
{
    String_FromChars(s, "水平線に一人きり　孤独に包み込まれて");
    String_AllocWordNum(&s, 0);
    String_AllocWordNum(&s, 0xff);
    //should be String_AllocWordNum(String *, size_t)
    //String_AllocWordNum(&s, -0xff);
    String_Dtor(&s);
}

void TestGetChar()
{
    String_FromChars(s, "Rocaloid");
    RAssert(String_GetChar(&s, 0) == 'R');
    String_Dtor(&s);
}

void TestSetChar()
{
    String_FromChars(s, "Vocaloid");
    String_SetChar(&s, 0, 'R');
    RAssert(String_GetChar(&s, 0) == 'R');
    String_Dtor(&s);
}

void TestGetWord()
{
    String tmp;
    String_Ctor(& tmp);
    String_FromChars(s, "夏は熱を持ってるのに　冷めたままで");
    
    String_GetWord(& s, & tmp, 2);
    RAssert(! (strwcmp_utf8("熱", tmp.Data, 1)));
    
    String_GetWord(& s, & tmp, 11);
    RAssert(! (strwcmp_utf8("冷", tmp.Data, 1)));
    
    String_Dtor(&s);
    String_Dtor(& tmp);
}

void TestSetWord()
{
    String_FromChars(s, "Uと出会えた夢たちが");
    String_FromChars(w, "君");
    String_SetWord(&s, 0, &w);
    
    RAssert(String_GetWordNum(&s) == 10);
    RAssert(! strcmp_utf8(String_GetChars(&s), "君と出会えた夢たちが"));
    
    String_Dtor(&w);
    String_Dtor(&s);
}


void TestJoin()
{
    String_FromChars(a, "「僕も君");
    String_FromChars(b, "を待ってたよ」");
    String_Join(&a, &b);

    RAssert(String_GetWordNum(&a) == 11);
    RAssert(! strcmp_utf8("「僕も君を待ってたよ」", String_GetChars(&a)));
    
    String_Dtor(&a);
    String_Dtor(&b);
}

void TestJoinChars()
{
    String_FromChars(s, "それからあたしは");
    String_JoinChars(&s, " 呆れるくらい");

    RAssert(String_GetWordNum(&s) == 15);
    RAssert(! strcmp_utf8("それからあたしは 呆れるくらい", 
                          String_GetChars(&s)));
    
    String_Dtor(&s);
}

void TestEqual()
{
    String_FromChars(a, "毎日毎日　君の元へ");
    String_FromChars(b, "君には外の話を　君はあたしに笑顔を");
    String_FromChars(x, "毎日毎日　君の元へ");
    String_FromChars(p, "毎日");
    
    RAssert(!String_Equal(&a, &b));
    RAssert(String_Equal(&a, &x));
    RAssert(String_EqualN(&a, &p, 2));
    
    String_Dtor(&a);
    String_Dtor(&b);
    String_Dtor(&x);
    String_Dtor(&p);
}

void TestEqualChars()
{
    String_FromChars(x, "外に出られない君は　世界に憧れてた");

    //should be bool String_EqualChars(String *, const char *)
    RAssert(String_EqualChars(&x, "外に出られない君は　世界に憧れてた"));
    RAssert(String_EqualNChars(&x, "外に出られない君は", 9));
    
    String_Dtor(&x);
}

void TestInStr()
{
    String_FromChars(s, "白い屋敷に二人きり  君に包まれて");
    //                               ^           ^
    
    String_FromChars(p, "に");
    RAssert(String_InStr(& s, & p) == 4);
    RAssert(String_InStrRev(& s, & p) == 12);
    String_Dtor(& p);
    
    String_Dtor(& s);
}

void TestMid()
{
    String d;
    String_Ctor(& d);
    String_FromChars(s, "こんな気持ちは一生に一度　それだけなの");
    
    Mid(&d, &s, 7, 5);
    RAssert(String_EqualChars(&d, "一生に一度"));
    
    MidFrom(&d, &s, 13);
    RAssert(String_EqualChars(&d, "それだけなの"));
    
    Mid(&d, &s, 0, 7);
    RAssert(String_EqualChars(&d, "こんな気持ちは"));
    
    Mid(&d, &s, 0, 0);
    RAssert(String_EqualChars(&d, ""));
    
    Mid(&d, &s, 19, 0);
    RAssert(String_EqualChars(&d, ""));
    
    String_Dtor(& s);
    String_Dtor(& d);
}

void TestLR()
{
    String_FromChars(s, "君と出会えた奇跡が　私に笑顔をくれる");
    String d;
    String_Ctor(& d);
    
    Left(&d, &s, 9);
    RAssert(String_EqualChars(&d, "君と出会えた奇跡が"));
    
    Right(&d, &s, 8);
    RAssert(String_EqualChars(&d, "私に笑顔をくれる"));
    
    Left(&d, &s, 0);
    RAssert(String_EqualChars(&d, ""));
    
    Right(&d, &s, 0);
    RAssert(String_EqualChars(&d, ""));
    
    String_Dtor(& s);
    String_Dtor(& d);
}

void TestTrim()
{
    String_FromChars(a, "    強く握った君の手が　いなくなった    ");
    String_FromChars(b, "    N   ");
    String_FromChars(c, "    桜   ");
    String d;
    String_Ctor(& d);
    
    Trim(& d, & a);
    RAssert(String_EqualChars(&d, "強く握った君の手が　いなくなった"));
    
    Trim(& d, & b);
    RAssert(String_EqualChars(&d, "N"));
    
    Trim(& d, & c);
    RAssert(String_EqualChars(&d, "桜"));
    
    LTrim(& d, & a);
    RAssert(String_EqualChars(&d, "強く握った君の手が　いなくなった    "));
    
    LTrim(& d, & b);
    RAssert(String_EqualChars(&d, "N   "));
    
    LTrim(& d, & c);
    RAssert(String_EqualChars(&d, "桜   "));
    
    RTrim(& d, & a);
    RAssert(String_EqualChars(&d, "    強く握った君の手が　いなくなった"));
    
    RTrim(& d, & b);
    RAssert(String_EqualChars(&d, "    N"));
    
    RTrim(& d, & c);
    RAssert(String_EqualChars(&d, "    桜"));
    
    String_Dtor(& d);
    String_Dtor(& c);
    String_Dtor(& b);
    String_Dtor(& a);
}

void TestCase()
{
    String_FromChars(a, "这是Test啊啊啊 HAHAHAHahaha!");
    String_FromChars(b, "");
    
    UpperCase(& b, & a);
    RAssert(String_EqualChars(& b, "这是TEST啊啊啊 HAHAHAHAHAHA!"));
    
    LowerCase(& b, & a);
    RAssert(String_EqualChars(& b, "这是test啊啊啊 hahahahahaha!"));
    
    String_Dtor(& b);
    String_Dtor(& a);
}

void TestWCMatch()
{
    String_FromChars(Str1, "WildCard!..loop");
    String_FromChars(Pat1, "*.loop");
    RAssert(Wildcard_Match(& Str1, & Pat1) == 1);
    
    String_FromChars(Str2, "通配符测试.txt");
    String_FromChars(Pat2_0, "*.bin");
    String_FromChars(Pat2_1, "*.txt");
    String_FromChars(Pat2_2, "通??测?.t*");
    RAssert(Wildcard_Match(& Str2, & Pat2_0) == 0);
    RAssert(Wildcard_Match(& Str2, & Pat2_1) == 1);
    RAssert(Wildcard_Match(& Str2, & Pat2_2) == 1);
    
    String_Dtor(& Str1);
    String_Dtor(& Str2);
    String_Dtor(& Pat1);
    String_Dtor(& Pat2_0);
    String_Dtor(& Pat2_1);
    String_Dtor(& Pat2_2);
}

int main()
{
    TestTor(); // Will great water wall block it? LOL
    TestFromChars();
    TestSetChars();
    TestGetChars();
    TestCopy();
    TestGetWordNum();
    TestAllocWordNum();
    TestGetChar();
    TestSetChar();
    TestGetWord();
    TestSetWord();
    TestJoin();
    TestJoinChars();
    TestEqual();
    TestEqualChars();
    TestInStr();
    TestMid();
    TestLR();
    TestTrim();
    TestCase();
    TestWCMatch();
    
    return 0;
}

#undef WITH_ASSERT_ABORT

