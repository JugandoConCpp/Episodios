#ifndef UTIL_H__INCLUDED
#define UTIL_H__INCLUDED

#include <print>
#include <string_view>
#include <string>

#include <testie.h>

#include "terminal.h"
#include "test_run_results.h"

extern "C"
{
     typedef size_t (*Get_Test_Group_Count_Type)();
     typedef Testie::Test_Group** (*Get_Test_Groups_Type)();
}

bool String_Starts_With(std::string_view string, std::string_view prefix)
{
     return (string.length() >= prefix.length()) && (string.substr(0, prefix.length()) == prefix);
}

void Print_Test_Results(std::string header, Test_Run_Results const& results)
{
     std::println("{}: {} tests run, " COLOR_GREEN "{} passed" COLOR_DEFAULT ", " COLOR_RED "{} failed" COLOR_DEFAULT,
          header,
          results.run_count,
          results.pass_count,
          results.fail_count);
}

#endif  // UTIL_H__INCLUDED
