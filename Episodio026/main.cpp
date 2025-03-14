#include "ini.h"

#include <print>
#include <string>
#include <string_view>

void Usage(std::string_view argv0)
{
     std::println("Usage:");
     std::println("");
     std::println("  {} <INF file>", argv0);
     std::println("");
}

int main(int argc, char* argv[])
{
     bool dump = false;
     std::string section;
     std::string name;

     if (argc == 2)
     {
          dump = true;
     } else
     if (argc == 3)
     {
          name = argv[2];
     } else
     if (argc == 4)
     {
          section = argv[2];
          name = argv[3];
     } else {
          Usage(argv[0]);
          return -1;
     }

     using Ini_Format::Ini;
     Ini ini = Ini::From_File(argv[1]);

     if (dump)
     {
          int line_number = 0;
          for (auto const& line: ini.Lines())
               std::println("{:5} {}", ++line_number, std::string_view{line});
     } else if (!name.empty()) {
          std::string value;
          bool found = ini.Find_Value(section, name, &value);

          if (found)
          {
               std::println(R"("{}" found in section "{}" with value "{}")", name, section, value);
          } else {
               std::println(R"("{}" not found in section "{}")", name, section);
          }
     }
     
     return 0;
}
