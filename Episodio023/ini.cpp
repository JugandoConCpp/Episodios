#define INI_FORMAT_API __declspec(dllexport)

#include "ini.h"

#include <cstdint>
#include <list>
#include <string>
#include <string_view>
#include <utility>

#include "line.h"

using Ini_Format::Ini;
using Ini_Format::Line;

std::list<Line>::const_iterator Ini::find_line_impl(uint64_t line_number, std::list<Line>::const_iterator section_start) const
{
     uint64_t current_line_number = 0;
     std::list<Line>::const_iterator it;

     for (it = section_start; it != _lines.end(); ++it)
     {
          if (current_line_number == line_number)
               return it;

          if (it->Is_Section_Header())
               return it;

          ++current_line_number;
     }

     return it;
}

std::list<Line>::const_iterator Ini::find_section_start(uint64_t index) const
{
     uint64_t current_section_index = -1;
     std::list<Line>::const_iterator it;

     for (it = _lines.begin(); it != _lines.end(); ++it)
     {
          if (it->Is_Section_Header())
               ++current_section_index;

          if (current_section_index == index)
               return it;
     }

     return it;
}

std::pair<uint64_t, std::list<Line>::const_iterator> Ini::find_section_start(std::string_view name) const
{
     uint64_t current_line_number = 0;
     std::list<Line>::const_iterator it;

     for (it = _lines.begin(); it != _lines.end(); ++it)
     {
          std::string found_section_name;

          if (it->Is_Section_Header(&found_section_name) && (found_section_name == name))
               return std::make_pair(current_line_number, it);

          ++current_line_number;
     }

     return std::make_pair(current_line_number, it);
}  // class Line

void Ini::Add_Value_To_Section(std::string_view section_name, uint64_t line_number, std::string_view value)
{
     auto [_, it] = find_section_start(section_name);

     if (it == _lines.end())
     {
          Add_Section(section_name);
          auto [_, it] = find_section_start(section_name);
     }

     _lines.insert(find_line_impl(line_number, ++it), std::string{value});
}
