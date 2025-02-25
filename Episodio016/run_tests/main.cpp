#include <algorithm>
#include <print>
#include <span>
#include <string_view>

#include <Windows.h>

#include <test_function.h>
#include <test_group.h>

#include "library.h"
#include "test_run_results.h"
#include "terminal.h"
#include "util.h"

Test_Run_Results Run_Test(char const* filename)
{
     std::println(COLOR_BLUE "*** Running Tests from: {} ***" COLOR_DEFAULT, filename);

     Library test(filename);

     Test_Run_Results file_results;

     size_t count;
     HRESULT result;

     result = test.Call_Function("Get_Test_Group_Count", count);
     if (FAILED(result))
     {
          std::println("Not a unit test.  Skipping.");
          return Test_Run_Results{};
     }

     Testie::Test_Group** test_group_array;

     result = test.Call_Function("Get_Test_Groups", test_group_array);
     if (FAILED(result))
     {
          std::println("Not a unit test.  Skipping.");
          return Test_Run_Results{};
     }

     std::span<Testie::Test_Group*> test_groups(test_group_array, count);

     for (auto const* test_group: test_groups)
     {
          std::println(ENDL COLOR_CYAN "Executing test group: {}" COLOR_DEFAULT, test_group->Name());

          Test_Run_Results group_results;

          for (auto const& test_function: test_group->Tests())
          {
               bool test_passed = test_function();
               ++group_results.run_count;

               if (test_passed)
               {
                    std::print(COLOR_GREEN "[PASSED]" COLOR_DEFAULT);
                    ++group_results.pass_count;
               } else {
                    std::print(COLOR_RED "[FAILED]" COLOR_DEFAULT);
                    ++group_results.fail_count;
               }

               std::println("\x1b[0m {}", test_function.Name());
          }

          Print_Test_Results(std::format(COLOR_CYAN "Group {} results", test_group->Name()), group_results);

          file_results+= group_results;
     }

     Print_Test_Results(std::format(ENDL COLOR_BLUE "File {} results", filename), file_results);

     return file_results;
}

Test_Run_Results Run_Tests(std::span<char const*> filenames)
{
     Test_Run_Results results;

     std::for_each(
          filenames.begin(), filenames.end(),
          [&results](char const* filename) { results+= Run_Test(filename); });

     Print_Test_Results("\x1b[1m" COLOR_BRIGHT_WHITE "Combined results", results);
     std::print("\x1b[22m");

     return results;
}

int main(int argc, char const* argv[])
{
     DWORD console_mode;
     HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

     if (!GetConsoleMode(console_handle, &console_mode))
     {
          //
          // Error.
          //

          return -1;
     }

     if (!SetConsoleMode(console_handle, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
     {
          //
          // Error.
          //

          return -1;
     }

     Test_Run_Results results = Run_Tests(std::span<char const*>(&argv[1], &argv[argc]));

     std::print("\x1b[0m");
     if (!SetConsoleMode(console_handle, console_mode))
     {
          //
          // Error.
          //

          return -1;
     }

     return results.fail_count;
}

