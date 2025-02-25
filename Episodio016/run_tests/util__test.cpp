#include "util.h"

#include "testie.h"

BEGIN_TEST_GROUP(String_Starts_With)
     DECLARE_TEST(Empty_Strings)
     DECLARE_TEST(String_Lengths)
     DECLARE_TEST(True_Prefixes)
END_TEST_GROUP(String_Starts_With)

DEFINE_TEST(String_Starts_With, Empty_Strings)
{
     VERIFY_IS_TRUE(String_Starts_With("", ""))
     VERIFY_IS_TRUE(String_Starts_With("i6ygquwerlfa", ""))
     VERIFY_IS_FALSE(String_Starts_With("", "97uqriejl"))

     return true;
}

DEFINE_TEST(String_Starts_With, String_Lengths)
{
     VERIFY_IS_FALSE(String_Starts_With("685431", "685431 "))
     VERIFY_IS_TRUE(String_Starts_With("685431", "685431"))

     return true;
}

DEFINE_TEST(String_Starts_With, True_Prefixes)
{
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", ""))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "g"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gy"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyq"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqo"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqol"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqoli"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqoli"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqolid"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqolidj"))
     VERIFY_IS_TRUE(String_Starts_With("gyqolidjk", "gyqolidjk"))
     VERIFY_IS_FALSE(String_Starts_With("gyqolidjk", "gyqolidjkp"))

     return true;
}

