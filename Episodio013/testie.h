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

extern "C" __declspec(dllexport) size_t Get_Test_Function_Count();
extern "C" __declspec(dllexport) Testie::Test_Function* Get_Test_Functions();
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

     extern std::vector<Test_Function> Test_Functions;  // deprecated
     extern std::vector<Test_Group*> Test_Groups;

     struct Add_Test
     {
          Add_Test(Test_Group& group, std::string const& name, Test_Group::Test_Function_Pointer pointer)
          {
               group.Add_Test(name, pointer);
          }
     };
}

#define UNIT_TEST(Name)                                                                        \
     std::vector<Testie::Test_Function> Testie::Test_Functions;                                \
     std::vector<Testie::Test_Group*> Testie::Test_Groups;                                     \
     extern "C" __declspec(dllexport) size_t Get_Test_Function_Count()  { return Testie::Test_Functions.size(); }         \
     extern "C" __declspec(dllexport) Testie::Test_Function* Get_Test_Functions() { return &Testie::Test_Functions[0]; }     \
     extern "C" __declspec(dllexport) size_t Get_Test_Group_Count()  { return Testie::Test_Groups.size(); }         \
     extern "C" __declspec(dllexport) Testie::Test_Group** Get_Test_Groups() { return &Testie::Test_Groups[0]; }     \

#define BEGIN_TEST_GROUP(Name)                                                            \
          struct Test_Group__##Name##__: Testie::Test_Group                               \
          {                                                                               \
               Test_Group__##Name##__():                                                  \
                    Test_Group(#Name)                                                     \
                    {                                                                     \
                         Testie::Test_Groups.push_back(this);                             \
                    }                                                                     \

#define END_TEST_GROUP(Name)                                                              \
          };                                                                              \
          static Test_Group__##Name##__ Test_Group__##Name##__instance;                   \
          

#define DECLARE_TEST(Name)                                                                \
     bool TEST__##Name##__ () const;                                                      \

#define DEFINE_TEST(GroupName, TestName)                                                  \
     static Testie::Add_Test                                                              \
     TEST__##GroupName##__##TestName##__adder(                                            \
          Test_Group__##GroupName##__instance,                                            \
          #TestName,                                                                      \
          [](){return Test_Group__##GroupName##__instance.TEST__##TestName##__();});      \
                                                                                          \
     bool Test_Group__##GroupName##__::TEST__##TestName##__() const                       \

#define VERIFY(Op) VERIFY_IS_TRUE(Op)
#define VERIFY_IS_TRUE(Op) if (!Testie::Verify(__FILE__, __LINE__, #Op, (Op))) return false;
#define VERIFY_IS_FALSE(Op) VERIFY_IS_TRUE((!(Op)))

#endif  // TESTIE__TESTIE_H__INCLUDED
