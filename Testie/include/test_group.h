#ifndef TESTIE__TEST_GROUP_H__INCLUDED
#define TESTIE__TEST_GROUP_H__INCLUDED

#include <functional>
#include <string>
#include <vector>

#include "test_function.h"

namespace Testie
{
     class Test_Group
     {
          private:
               std::string const _name;
               std::vector<Test_Function> _tests;

          public:
               Test_Group(std::string const& name):
                    _name{name},
                    _tests{}
               {}

               void Add_Test(std::string const& name, Test_Function::Type function)
               {
                    _tests.emplace_back(name, function);
               }

               std::string const& Name() const
               {
                    return _name;
               }

               std::vector<Test_Function> const& Tests() const
               {
                    return _tests;
               }
     };  // class Test_Group
}  // namespace Testie

#endif  // TESTIE__TEST_GROUP_H__INCLUDED
