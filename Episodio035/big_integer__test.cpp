#include "big_integer.h"

#include <testie.h>

#include <print>

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

UNIT_TEST(Big_Integer_Tests)

BEGIN_TEST_GROUP(Big_Integer)
     DECLARE_TEST(Default_Constructor)
     DECLARE_TEST(Zero_Constructed)
     DECLARE_TEST(Nonzero_Constructed)
     DECLARE_TEST(Subtraction)
     DECLARE_TEST(Subtract_Self)
     DECLARE_TEST(Inversion)
     DECLARE_TEST(Add_Inverse)
     DECLARE_TEST(Sum)
END_TEST_GROUP(Big_Integer)

DEFINE_TEST(Big_Integer, Default_Constructor)
{
     Big_Integer bi;

     VERIFY_IS_TRUE(bi.Is_Zero());

     return true;
}

DEFINE_TEST(Big_Integer, Zero_Constructed)
{
     Big_Integer bi{0};

     VERIFY_IS_TRUE(bi.Is_Zero());

     return true;
}

DEFINE_TEST(Big_Integer, Nonzero_Constructed)
{
     int r1 = rand(), r2 = rand();

     uint64_t value = (uint64_t(r1) << 32) | uint64_t(r2);

     Big_Integer bi{value};

     VERIFY_IS_FALSE(bi.Is_Zero());
     VERIFY_IS_TRUE(bi == value);
     VERIFY_IS_TRUE(bi == Big_Integer{value});

     return true;
}

DEFINE_TEST(Big_Integer, Subtraction)
{
     Big_Integer bi_1{0x1234556};
     Big_Integer bi_2{ 0x234556};
     Big_Integer bi_3{0x1000000};

     Big_Integer bi_1_copy = bi_1;
     bi_1_copy+= -bi_2;
     VERIFY_IS_TRUE(bi_1_copy == bi_3);

     VERIFY_IS_TRUE((bi_1 - bi_2) == bi_3);

     return true;
}

DEFINE_TEST(Big_Integer, Subtract_Self)
{
     Big_Integer bi_1{0x1234556};

     VERIFY_IS_TRUE((bi_1 - bi_1) == 0);

     return true;
}

DEFINE_TEST(Big_Integer, Inversion)
{
     Big_Integer bi_1{0x1234556};
     Big_Integer bi_2(-0x1234556);

     VERIFY_IS_FALSE(bi_1 == bi_2);
     VERIFY_IS_TRUE(-bi_1 == bi_2);

     VERIFY_IS_FALSE(bi_2 == bi_1);
     VERIFY_IS_TRUE(-bi_2 == bi_1);

     return true;
}

DEFINE_TEST(Big_Integer, Add_Inverse)
{
     Big_Integer bi_1{0x1234556};
     Big_Integer bi_2(-0x1234556);
     Big_Integer bi_3 = bi_1 + bi_2;

     VERIFY_IS_TRUE(bi_3.Is_Zero());

     return true;
}

DEFINE_TEST(Big_Integer, Sum)
{
     Big_Integer bi_1{0x123};
     Big_Integer bi_2{0xfed};
     Big_Integer bi_3 = bi_1 + bi_2;

     Big_Integer bi_1_copy = bi_1;
     bi_1_copy+= bi_2;

     VERIFY_IS_TRUE(bi_1_copy == Big_Integer{0x1110});
     VERIFY_IS_TRUE(bi_3 == Big_Integer{0x1110});

     return true;
}
