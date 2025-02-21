#ifndef TESTIE__TESTIE_H__INCLUDED
#define TESTIE__TESTIE_H__INCLUDED

#include <print>
#include <string_view>
#include <vector>

typedef bool(*Test_Function_Type)();

extern "C" __declspec(dllexport) size_t Get_Test_Function_Count();
extern "C" __declspec(dllexport) Test_Function_Type* Get_Test_Functions();

namespace Testie
{
     inline bool Verify(std::string_view file, int line, std::string_view expression, bool value)
     {
          if (!value)
          {
               std::print(R"([{0}:{1}] Failed expression: {2})", file, line, expression);
               return false;
          }

          return true;
     }

     extern std::vector<Test_Function_Type> Test_Functions;

     struct Add_Test { Add_Test(Test_Function_Type pointer) { Test_Functions.push_back(pointer); } };
}

#define UNIT_TEST(Name)                                                                             \
     std::vector<Test_Function_Type> Testie::Test_Functions;                                        \
     extern "C" __declspec(dllexport) size_t Get_Test_Function_Count()  { return Testie::Test_Functions.size(); }         \
     extern "C" __declspec(dllexport) Test_Function_Type* Get_Test_Functions() { return &Testie::Test_Functions[0]; }     \

#define TEST(Name)                                                    \
     extern "C" bool TEST__##Name##__ ();                             \
     static Testie::Add_Test TEST__##Name##__adder(TEST__##Name##__); \
     extern "C" bool TEST__##Name##__ ()                              \

#define VERIFY(Op) VERIFY_IS_TRUE(Op)
#define VERIFY_IS_TRUE(Op) if (!Testie::Verify(__FILE__, __LINE__, #Op, (Op))) return false;
#define VERIFY_IS_FALSE(Op) VERIFY_IS_TRUE((!(Op)))

#endif  // TESTIE__TESTIE_H__INCLUDED
