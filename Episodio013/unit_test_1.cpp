#include <string>

#include "testie.h"

UNIT_TEST(Unit_Test_1)

BEGIN_TEST_GROUP(Numbers)
     DECLARE_TEST(Sumas_Y_Restas)
     DECLARE_TEST(Multiplicaciones_Y_Divisiones)
END_TEST_GROUP(Numbers);

BEGIN_TEST_GROUP(Texto)
     DECLARE_TEST(Cadenas)
END_TEST_GROUP(Texto);

DEFINE_TEST(Numbers, Sumas_Y_Restas)
{
     VERIFY(3 + 3 == 6);
     VERIFY(3 + 4 == 7);

     return true;
}

DEFINE_TEST(Numbers, Multiplicaciones_Y_Divisiones)
{
     VERIFY(3 * 3 == 9);
     VERIFY(3 / 3 == 1);

     return true;
}

DEFINE_TEST(Texto, Cadenas)
{
     char const* s1 = "abc";
     char const* s2 = "abcd";
     VERIFY(s1 == s2);
     VERIFY(std::string("abc") == "abc");

     return true;
}
