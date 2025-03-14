#define INI_FORMAT_API __declspec(dllexport)

#include "section.h"

using Ini_Format::Line;
using Ini_Format::Section;

using Ini_Format::pos_t;

Section::Section(std::string_view name):
     _name(name)
{
     if (name.empty())
          return;

     if (Line::Is_Section_Header(name))
     {
          _lines.emplace_back(name);
     } else {
          _lines.emplace_back(Line::Make_Section_Header(name));
     }
}

void Section::Update_Name()
{
     if (_lines.empty())
          return;

     _lines.front().Is_Section_Header(&_name);
}

std::pair<Section, pos_t>
Section::From_Buffer(std::string_view buffer)
{
     Section section;
     bool first = true;
     pos_t pos = 0;

     for (; pos < buffer.length(); pos+= 2)
     {
          bool end = false;
          pos_t next_pos = buffer.find("\r\n", pos);

          if (next_pos == std::string_view::npos)
          {
               end = true;
               next_pos = buffer.length();
          }

          Line line = buffer.substr(pos, (next_pos - pos));

          if (line.Is_Section_Header() && !first)
               break;

          section.Add_Line(line);
          pos = next_pos;
          first = false;

          if (end)
               break;
     }

     section.Update_Name();

     return std::make_pair(section, pos);
}

Section::Iterator
Section::find_line_impl(
          uint64_t line_number) const
{
     uint64_t current_line_number = 0;
     Section::Iterator it;

     for (it = _lines.begin(); it != _lines.end(); ++it)
     {
          if (current_line_number == line_number)
               return it;

          ++current_line_number;
     }

     return it;
}

