#include <algorithm>
#include <print>
#include <span>
#include <string_view>

#include <Windows.h>

#include "test_function.h"

extern "C"
{
     typedef size_t (*Get_Test_Function_Count_Type)();
     typedef Testie::Test_Function* (*Get_Test_Functions_Type)();
}

bool String_Starts_With(std::string_view string, std::string_view prefix)
{
     return (string.length() >= prefix.length()) && (string.substr(0, prefix.length()) == prefix);
}

int Run_Test(char const* filename)
{
     std::println("Running Tests from: {}", filename);

     auto Test = LoadLibrary(filename);

     Get_Test_Function_Count_Type Get_Test_Function_Count = (Get_Test_Function_Count_Type) GetProcAddress(Test, "Get_Test_Function_Count");
     if (!Get_Test_Function_Count)
     {
          std::println("Not a unit test.  Skipping.");
          return 0;
     }

     Get_Test_Functions_Type Get_Test_Functions = (Get_Test_Functions_Type) GetProcAddress(Test, "Get_Test_Functions");
     if (!Get_Test_Functions)
     {
          std::println("Not a unit test.  Skipping.");
          return 0;
     }

     size_t count = (*Get_Test_Function_Count)();
     std::println("Unit test has {} tests.", count);

     std::span<Testie::Test_Function> test_functions((*Get_Test_Functions)(), count);

     int run_count = 0;
     int pass_count = 0;
     int fail_count = 0;

     for (auto const& test_function: test_functions)
     {
          bool test_passed = test_function();
          ++run_count;

          if (test_passed)
          {
               std::print("\x1b[1;32m[PASSED]\x1b[0m");
               ++pass_count;
          } else {
               std::print("\x1b[1;31m[FAILED]\x1b[0m");
               ++fail_count;
          }

          std::println("\x1b[0m {}", test_function.Name());
     }

     std::println("{} test run, {} passed, {} failed", run_count, pass_count, fail_count);

     FreeLibrary(Test);

     return fail_count;
}

int Run_Tests(std::span<char const*> filenames)
{
     int fail_count = 0;

     std::for_each(
          filenames.begin(), filenames.end(),
          [&fail_count](char const* filename) { fail_count+= Run_Test(filename); });

     return fail_count;
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

     int fail_count = Run_Tests(std::span<char const*>(&argv[1], &argv[argc]));

     std::print("\x1b[0m");
     if (!SetConsoleMode(console_handle, console_mode))
     {
          //
          // Error.
          //

          return -1;
     }

     return fail_count;
}

