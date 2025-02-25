#include "test_run_results.h"

#include <testie.h>

UNIT_TEST(Run_Tests)

BEGIN_TEST_GROUP(Test_Run_Results)
     DECLARE_TEST(Default_Constructor)
     DECLARE_TEST(Value_Constructor)
     DECLARE_TEST(Copy_Constructor)
     DECLARE_TEST(Move_Constructor)
END_TEST_GROUP(Test_Run_Results)

DEFINE_TEST(Test_Run_Results, Default_Constructor)
{
     Test_Run_Results o;

     VERIFY_ARE_EQUAL(0, o.run_count);
     VERIFY_ARE_EQUAL(0, o.pass_count);
     VERIFY_ARE_EQUAL(0, o.fail_count);

     return true;
}

DEFINE_TEST(Test_Run_Results, Value_Constructor)
{
     Test_Run_Results o(138, 834, 89671);

     VERIFY_ARE_EQUAL(138, o.run_count);
     VERIFY_ARE_EQUAL(834, o.pass_count);
     VERIFY_ARE_EQUAL(89671, o.fail_count);

     return true;
}

DEFINE_TEST(Test_Run_Results, Copy_Constructor)
{
     Test_Run_Results o1;
     Test_Run_Results o2 = o1;

     VERIFY_ARE_EQUAL(o1.run_count, o2.run_count);
     VERIFY_ARE_EQUAL(o1.pass_count, o2.pass_count);
     VERIFY_ARE_EQUAL(o1.fail_count, o2.fail_count);

     o1.run_count = 25;
     o1.pass_count = 153;
     o1.fail_count = 35;

     VERIFY_ARE_NOT_EQUAL(o1.run_count, o2.run_count);
     VERIFY_ARE_NOT_EQUAL(o1.pass_count, o2.pass_count);
     VERIFY_ARE_NOT_EQUAL(o1.fail_count, o2.fail_count);

     Test_Run_Results o3 = o1;

     VERIFY_ARE_EQUAL(o1.run_count, o3.run_count);
     VERIFY_ARE_EQUAL(o1.pass_count, o3.pass_count);
     VERIFY_ARE_EQUAL(o1.fail_count, o3.fail_count);

     return true;
}

DEFINE_TEST(Test_Run_Results, Move_Constructor)
{
     Test_Run_Results o1 = Test_Run_Results{};

     VERIFY_ARE_EQUAL(0, o1.run_count);
     VERIFY_ARE_EQUAL(0, o1.pass_count);
     VERIFY_ARE_EQUAL(0, o1.fail_count);

     Test_Run_Results o2(138, 12, 861);

     VERIFY_ARE_EQUAL(138, o2.run_count);
     VERIFY_ARE_EQUAL(12, o2.pass_count);
     VERIFY_ARE_EQUAL(861, o2.fail_count);

     return true;
}
