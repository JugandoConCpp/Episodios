#ifndef TESTIE__TEST_FUNCTION_H__INCLUDED
#define TESTIE__TEST_FUNCTION_H__INCLUDED

#include <functional>
#include <string>
#include <vector>

namespace Testie
{
     class Test_Function
     {
          public:
               typedef std::function<bool()> Type;

          private:
               std::string _name;
               Type _function;

          public:
               Test_Function(std::string name, Type function):
                    _name{name},
                    _function{function}
               {}

               std::string const& Name() const { return _name; }
               Type Function() const { return _function; }

               bool operator()() const { return _function(); }
     };
}

#endif  // TESTIE__TEST_FUNCTION_H__INCLUDED
