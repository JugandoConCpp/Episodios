#define INI_FORMAT_API __declspec(dllexport)

#include "ini.h"

#include <cstdint>
#include <fstream>
#include <list>
#include <string>
#include <string_view>
#include <utility>

#include "line.h"

using Ini_Format::Ini;
using Ini_Format::Line;

INI_FORMAT_API
Ini
Ini::From_File(std::string const& filename)
{
     std::ifstream stream(filename, std::ios_base::binary | std::ios_base::in | std::ios_base::ate);

     if (!stream)
          return Ini{};

     auto file_size = uint32_t(stream.tellg());
     std::string buffer(file_size, '\0');
     stream.seekg(0);

     uint32_t total_bytes_read = 0;

     while (total_bytes_read < file_size)
     {
          stream.read(&buffer[0], file_size - total_bytes_read);
          total_bytes_read+= uint32_t(stream.gcount());
     }

     return From_Buffer(buffer);
}

INI_FORMAT_API
Ini
Ini::From_Buffer(std::string_view buffer)
{
     using pos_t = std::string::size_type;

     Ini ini;

     for (pos_t pos = 0;; pos+= 2)
     {
          pos_t next_pos = buffer.find("\r\n", pos);

          if (next_pos != std::string::npos)
          {
               ini.Add_Line(buffer.substr(pos, (next_pos - pos)));
               pos = next_pos;
          } else {
               ini.Add_Line(buffer.substr(pos));
               break;
          }
     }

     return ini;
}

std::list<Line>::const_iterator
Ini::find_line_impl(
          uint64_t line_number,
          std::list<Line>::const_iterator section_start,
          bool stop_on_section_start) const
{
     uint64_t current_line_number = 0;
     std::list<Line>::const_iterator it;

     for (it = section_start; it != _lines.end(); ++it)
     {
          if (current_line_number == line_number)
               return it;

          if (stop_on_section_start && it->Is_Section_Header())
               return it;

          ++current_line_number;
     }

     return it;
}

std::list<Line>::const_iterator
Ini::find_section_start(uint64_t index) const
{
     uint64_t current_section_index = 0;
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

std::pair<uint64_t, std::list<Line>::const_iterator>
Ini::find_section_start(std::string_view name) const
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

     _lines.insert(find_line_impl(line_number, ++it, true), std::string{value});
}
