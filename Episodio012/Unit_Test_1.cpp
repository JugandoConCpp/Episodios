#include <string>

#include "Testie.h"

UNIT_TEST(Unit_Test_1)

TEST(Sumas_Y_Restas)
{
     VERIFY(3 + 3 == 6);
     VERIFY(3 + 4 == 7);

     return true;
}

TEST(Multiplicaciones_Y_Divisiones)
{
     VERIFY(3 * 3 == 9);
     VERIFY(3 / 3 == 1);

     return true;
}

TEST(Cadenas)
{
     char const* s1 = "abc";
     char const* s2 = "abcd";
     VERIFY(s1 == s2);
     VERIFY(std::string("abc") == "abc");

     return true;
}
