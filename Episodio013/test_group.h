#ifndef TEST_GROUP_H__INCLUDED
#define TEST_GROUP_H__INCLUDED

#include <functional>
#include <string>
#include <vector>

namespace Testie
{
     class Test_Group
     {
          public:
               typedef std::function<bool()> Test_Function_Pointer;

          private:
               struct Test_Function {
                    std::string const Name;
                    Test_Function_Pointer const Pointer;
                    bool operator()() const { return Pointer(); }
               };

               std::string const _name;
               std::vector<Test_Function> _tests;

          public:
               Test_Group(std::string const& name):
                    _name{name},
                    _tests{}
               {}

               void Add_Test(std::string const& name, Test_Function_Pointer pointer)
               {
                    _tests.emplace_back(Test_Function{ .Name = name, .Pointer = pointer});
               }

               std::string const& Name() const
               {
                    return _name;
               }

               std::vector<Test_Function> const& Tests() const
               {
                    return _tests;
               }
     };
}

#endif  // TEST_GROUP_H__INCLUDED
