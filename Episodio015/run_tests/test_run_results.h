#ifndef TEST_TRUN_RESULTS_H__INCLUDED
#define TEST_TRUN_RESULTS_H__INCLUDED

struct Test_Run_Results
{
     int run_count;
     int pass_count;
     int fail_count;

     Test_Run_Results(): Test_Run_Results(0, 0, 0) {}

     Test_Run_Results(int run, int pass, int fail):
          run_count{run},
          pass_count{pass},
          fail_count{fail}
     {}

     Test_Run_Results(Test_Run_Results const&) = default;
     Test_Run_Results(Test_Run_Results&&) = default;

     Test_Run_Results operator+(Test_Run_Results const& that) const
     {
          Test_Run_Results result = *this;
          result+= that;
          return result;
     }

     Test_Run_Results operator+=(Test_Run_Results const& that)
     {
          run_count+= that.run_count;
          pass_count+= that.pass_count;
          fail_count+= that.fail_count;
          return *this;
     }
};  // Test_Run_Results

#endif  // TEST_TRUN_RESULTS_H__INCLUDED
