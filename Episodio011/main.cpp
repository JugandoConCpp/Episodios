#include <algorithm>
#include <print>
#include <span>
#include <string_view>

#include <Windows.h>

extern "C"
{
     typedef bool(*Test_Function_Type)();

     typedef size_t (*Get_Test_Function_Count_Type)();
     typedef Test_Function_Type* (*Get_Test_Functions_Type)();
}

bool String_Starts_With(std::string_view string, std::string_view prefix)
{
     return (string.length() >= prefix.length()) && (string.substr(0, prefix.length()) == prefix);
}

void Run_Test(char* filename)
{
     std::println("Running Tests from: {}", filename);

     auto Test = LoadLibrary(filename);

     Get_Test_Function_Count_Type Get_Test_Function_Count = (Get_Test_Function_Count_Type) GetProcAddress(Test, "Get_Test_Function_Count");
     if (!Get_Test_Function_Count)
     {
          std::println("Not a unit test.  Skipping.");
          return;
     }

     Get_Test_Functions_Type Get_Test_Functions = (Get_Test_Functions_Type) GetProcAddress(Test, "Get_Test_Functions");
     if (!Get_Test_Functions)
     {
          std::println("Not a unit test.  Skipping.");
          return;
     }

     size_t count = (*Get_Test_Function_Count)();
     std::println("Unit test has {} tests.", count);

     std::span<Test_Function_Type> test_functions((*Get_Test_Functions)(), count);

     for (auto test_function: test_functions)
     {
          (*test_function)();
     }

     FreeLibrary(Test);
}

void Run_Tests(std::span<char*> filenames)
{
     std::for_each(filenames.begin(), filenames.end(), &Run_Test);
}

int main(int argc, char* argv[])
{
     Run_Tests(std::span(&argv[1], &argv[argc]));

     return 0;
}

