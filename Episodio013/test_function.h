#include <string>
#include <vector>

typedef bool(*Test_Function_Type)();

namespace Testie
{
     class Test_Function
     {
          private:
               std::string _name;
               Test_Function_Type _function;

          public:
               Test_Function(std::string name, Test_Function_Type function):
                    _name{name},
                    _function{function}
               {}

               std::string const& Name() const { return _name; }
               Test_Function_Type Function() const { return _function; }

               bool operator()() const { return (*_function)(); }
     };
}
