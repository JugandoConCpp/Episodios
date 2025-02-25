#ifndef TESTIE__TESTIE_H__INCLUDED
#define TESTIE__TESTIE_H__INCLUDED

#include <functional>
#include <print>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "test_function.h"
#include "test_group.h"

extern "C" __declspec(dllexport) size_t Get_Test_Group_Count();
extern "C" __declspec(dllexport) Testie::Test_Group** Get_Test_Groups();

namespace Testie
{
     inline bool Verify(std::string_view file, int line, std::string_view expression, bool value)
     {
          if (!value)
          {
               std::println("[{0}:{1}] Failed expression: {2}", file, line, expression);
               return false;
          }

          return true;
     }

     extern std::vector<Test_Group*> Test_Groups;

     struct Add_Test
     {
          Add_Test(Test_Group& group, std::string const& name, Test_Function::Type function)
          {
               group.Add_Test(name, function);
          }
     };
}

#define UNIT_TEST(Name)                                                                        \
     std::vector<Testie::Test_Group*> Testie::Test_Groups;                                     \
     extern "C" __declspec(dllexport) size_t Get_Test_Group_Count()  { return Testie::Test_Groups.size(); }         \
     extern "C" __declspec(dllexport) Testie::Test_Group** Get_Test_Groups() { return &Testie::Test_Groups[0]; }     \

#define TEST_GROUP_NAME(Name) Test_Group__##Name##__
#define TEST_GROUP_INSTANCE_NAME(Name) Test_Group__##Name##__##instance

#define TEST_FUNCTION_NAME(Name) TEST__##Name##__

#define BEGIN_TEST_GROUP(Name)                                                                      \
          struct TEST_GROUP_NAME(Name): Testie::Test_Group                                          \
          {                                                                                         \
               TEST_GROUP_NAME(Name)():                                                             \
                    Test_Group(#Name)                                                               \
                    {                                                                               \
                         Testie::Test_Groups.push_back(this);                                       \
                    }                                                                               \

#define END_TEST_GROUP(Name)                                                                        \
          };                                                                                        \
          static TEST_GROUP_NAME(Name) TEST_GROUP_INSTANCE_NAME(Name);                              \
          

#define DECLARE_TEST(Name)                                                                          \
     bool TEST_FUNCTION_NAME(Name)() const;                                                         \

#define DEFINE_TEST(GroupName, TestName)                                                            \
     static Testie::Add_Test                                                                        \
     TEST__##GroupName##__##TestName##__adder(                                                      \
          TEST_GROUP_INSTANCE_NAME(GroupName),                                                      \
          #TestName,                                                                                \
          [](){return TEST_GROUP_INSTANCE_NAME(GroupName).TEST_FUNCTION_NAME(TestName)();});        \
                                                                                                    \
     bool TEST_GROUP_NAME(GroupName)::TEST_FUNCTION_NAME(TestName)() const                          \

#define VERIFY(Op) VERIFY_IS_TRUE(Op)
#define VERIFY_IS_TRUE(Op) if (!Testie::Verify(__FILE__, __LINE__, #Op, (Op))) return false;
#define VERIFY_IS_FALSE(Op) VERIFY_IS_TRUE((!(Op)))

#define VERIFY_ARE_EQUAL(expected, actual) VERIFY_IS_TRUE((actual == expected))
#define VERIFY_ARE_NOT_EQUAL(expected, actual) VERIFY_IS_TRUE((actual != expected))

#endif  // TESTIE__TESTIE_H__INCLUDED
