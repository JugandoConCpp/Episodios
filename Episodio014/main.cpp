#include <algorithm>
#include <print>
#include <span>
#include <string_view>

#include <Windows.h>

#include "test_function.h"
#include "test_group.h"

#define ENDL "\n"

#define COLOR(n) "\x1b[1;" #n "m"

#define COLOR_BLACK           COLOR(30)
#define COLOR_RED             COLOR(31)
#define COLOR_GREEN           COLOR(32)
#define COLOR_YELLOW          COLOR(33)
#define COLOR_BLUE            COLOR(34)
#define COLOR_MAGENTA         COLOR(35)
#define COLOR_CYAN            COLOR(36)
#define COLOR_WHITE           COLOR(37)
#define COLOR_DEFAULT         COLOR(39)

#define COLOR_BRIGHT_BLACK    COLOR(90)
#define COLOR_BRIGHT_RED      COLOR(91)
#define COLOR_BRIGHT_GREEN    COLOR(92)
#define COLOR_BRIGHT_YELLOW   COLOR(93)
#define COLOR_BRIGHT_BLUE     COLOR(94)
#define COLOR_BRIGHT_MAGENTA  COLOR(95)
#define COLOR_BRIGHT_CYAN     COLOR(96)
#define COLOR_BRIGHT_WHITE    COLOR(97)

extern "C"
{
     typedef size_t (*Get_Test_Group_Count_Type)();
     typedef Testie::Test_Group** (*Get_Test_Groups_Type)();
}

bool String_Starts_With(std::string_view string, std::string_view prefix)
{
     return (string.length() >= prefix.length()) && (string.substr(0, prefix.length()) == prefix);
}

struct Test_Run_Results
{
     int run_count;
     int pass_count;
     int fail_count;

     Test_Run_Results():
          run_count{},
          pass_count{},
          fail_count{}
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


void Print_Test_Results(std::string header, Test_Run_Results const& results)
{
     std::println("{}: {} tests run, " COLOR_GREEN "{} passed" COLOR_DEFAULT ", " COLOR_RED "{} failed" COLOR_DEFAULT,
          header,
          results.run_count,
          results.pass_count,
          results.fail_count);
}

Test_Run_Results Run_Test(char const* filename)
{
     std::println(COLOR_BLUE "*** Running Tests from: {} ***" COLOR_DEFAULT, filename);

     auto Test = LoadLibrary(filename);

     Get_Test_Group_Count_Type Get_Test_Group_Count = (Get_Test_Group_Count_Type) GetProcAddress(Test, "Get_Test_Group_Count");
     if (!Get_Test_Group_Count)
     {
          std::println("Not a unit test.  Skipping.");
          return Test_Run_Results{};
     }

     Get_Test_Groups_Type Get_Test_Groups = (Get_Test_Groups_Type) GetProcAddress(Test, "Get_Test_Groups");
     if (!Get_Test_Groups)
     {
          std::println("Not a unit test.  Skipping.");
          return Test_Run_Results{};
     }

     Test_Run_Results file_results;

     size_t count = (*Get_Test_Group_Count)();
     std::span<Testie::Test_Group*> test_groups((*Get_Test_Groups)(), count);

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

     FreeLibrary(Test);

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

