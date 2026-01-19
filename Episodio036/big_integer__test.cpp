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
     DECLARE_TEST(List_Constructed)
     DECLARE_TEST(Subtraction)
     DECLARE_TEST(Subtract_Self)
     DECLARE_TEST(Inversion)
     DECLARE_TEST(Add_Inverse)
     DECLARE_TEST(Sum)
     DECLARE_TEST(Sum_Positive_Negative)
     DECLARE_TEST(Sum_Negative_Positive)
     DECLARE_TEST(Multiplication_Zero)
     DECLARE_TEST(Multiplication_One)
     DECLARE_TEST(Multiplication_Factorial)
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

DEFINE_TEST(Big_Integer, List_Constructed)
{
     Big_Integer bi{{0x123, 0x456, 0xffffffff}};

     VERIFY_IS_TRUE(bi.digit(0) == 0x123);
     VERIFY_IS_TRUE(bi.digit(1) == 0x456);
     VERIFY_IS_TRUE(bi.digit(2) == 0xffffffff);

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

DEFINE_TEST(Big_Integer, Sum_Positive_Negative)
{
     Big_Integer Zero{};

     Big_Integer bi_1{0x123};
     Big_Integer bi_2{0xfffffffffffffedd};
     Big_Integer bi_3 = bi_1 + bi_2;

     Big_Integer bi_1_copy = bi_1;
     bi_1_copy+= bi_2;

     VERIFY_IS_TRUE(bi_1_copy == Zero);
     VERIFY_IS_TRUE(bi_3 == Zero);

     return true;
}

DEFINE_TEST(Big_Integer, Sum_Negative_Positive)
{
     Big_Integer Zero{};

     Big_Integer bi_1{0xfffffffffffffedd};
     Big_Integer bi_2{0x123};
     Big_Integer bi_3 = bi_1 + bi_2;

     Big_Integer bi_1_copy = bi_1;
     bi_1_copy+= bi_2;

     VERIFY_IS_TRUE(bi_1_copy == Zero);
     VERIFY_IS_TRUE(bi_3 == Zero);

     return true;
}

DEFINE_TEST(Big_Integer, Multiplication_Zero)
{
     Big_Integer bi_1{0x123};
     Big_Integer Zero{};
     Big_Integer bi_3 = bi_1 * Zero;

     VERIFY_IS_TRUE(bi_3 == Zero);

     return true;
}

DEFINE_TEST(Big_Integer, Multiplication_One)
{
     Big_Integer bi_1{0x123};
     Big_Integer One{1};
     Big_Integer bi_3 = bi_1 * One;

     VERIFY_IS_TRUE(bi_3 == bi_1);

     return true;
}

DEFINE_TEST(Big_Integer, Multiplication_Factorial)
{
     auto Zero_Factorial = Big_Integer{0}.Factorial();
     auto One_Factorial = Big_Integer{1}.Factorial();
     auto Two_Factorial = Big_Integer{2}.Factorial();
     auto Three_Factorial = Big_Integer{3}.Factorial();
     auto Four_Factorial = Big_Integer{4}.Factorial();
     auto Five_Factorial = Big_Integer{5}.Factorial();
     auto Six_Factorial = Big_Integer{6}.Factorial();
     auto Seven_Factorial = Big_Integer{7}.Factorial();
     auto Eight_Factorial = Big_Integer{8}.Factorial();
     auto Nine_Factorial = Big_Integer{9}.Factorial();
     auto Ten_Factorial = Big_Integer{10}.Factorial();
     auto Eleven_Factorial = Big_Integer{11}.Factorial();
     auto Twelve_Factorial = Big_Integer{12}.Factorial();
     auto Thirteen_Factorial = Big_Integer{13}.Factorial();
     auto Fourteen_Factorial = Big_Integer{14}.Factorial();
     auto Fifteen_Factorial = Big_Integer{15}.Factorial();
     auto Sixteen_Factorial = Big_Integer{16}.Factorial();
     auto Seventeen_Factorial = Big_Integer{17}.Factorial();
     auto Eighteen_Factorial = Big_Integer{18}.Factorial();
     auto Nineteen_Factorial = Big_Integer{19}.Factorial();
     auto Twenty_Factorial = Big_Integer{20}.Factorial();

     VERIFY_IS_TRUE(Zero_Factorial ==                     1);
     VERIFY_IS_TRUE(One_Factorial ==                      1);
     VERIFY_IS_TRUE(Two_Factorial ==                      2);
     VERIFY_IS_TRUE(Three_Factorial ==                    6);
     VERIFY_IS_TRUE(Four_Factorial ==                    24);
     VERIFY_IS_TRUE(Five_Factorial ==                   120);
     VERIFY_IS_TRUE(Six_Factorial ==                    720);
     VERIFY_IS_TRUE(Seven_Factorial ==                 5040);
     VERIFY_IS_TRUE(Eight_Factorial ==                40320);
     VERIFY_IS_TRUE(Nine_Factorial ==                362880);
     VERIFY_IS_TRUE(Ten_Factorial ==                3628800);
     VERIFY_IS_TRUE(Eleven_Factorial ==           0x2611500);  //         39,916,800
     VERIFY_IS_TRUE(Twelve_Factorial ==          0x1C8CFC00);  //        479,001,600
     VERIFY_IS_TRUE(Thirteen_Factorial ==       0x17328CC00);  //      6,227,020,800
     VERIFY_IS_TRUE(Fourteen_Factorial ==      0x144C3B2800);  //     87,178,291,200
     VERIFY_IS_TRUE(Fifteen_Factorial ==      0x13077775800);  //  1,307,674,368,000
     VERIFY_IS_TRUE(Sixteen_Factorial ==     0x130777758000);  // 20,922,789,888,000
     VERIFY_IS_TRUE(Seventeen_Factorial ==  0x1437EEECD8000);  // 355,687,428,096,000
     VERIFY_IS_TRUE(Eighteen_Factorial ==  0x16BEECCA730000);  // 6,402,373,705,728,000
     VERIFY_IS_TRUE(Nineteen_Factorial == 0x1B02B9306890000);  // 121,645,100,408,832,000
     VERIFY_IS_TRUE(Twenty_Factorial ==  0x21C3677C82B40000);  // 2,432,902,008,176,640,000

     return true;
}
