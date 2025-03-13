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
using Ini_Format::Section;

using Ini_Format::pos_t;

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
     Ini ini;

     while (!buffer.empty())
     {
          auto [section, new_pos] = Section::From_Buffer(buffer);

          ini.Add_Section(std::move(section));
          buffer = buffer.substr(new_pos);
     }

     return ini;
}

Ini::Iterator
Ini::find_section_start(uint64_t index) const
{
     auto it = _sections.begin();

     for (int i = 0; i < index; ++i)
     {
          if (it == _sections.end())
               break;
          else
               ++it;
     }

     return it;
}

std::pair<uint64_t, Ini::Iterator>
Ini::find_section_start(std::string_view name) const
{
     int line_count = 0;
     auto it = _sections.begin();

     for (; it != _sections.end(); ++it)
     {
          uint64_t temp_line_count = line_count + it->Line_Count();

          if (it->Name() == name)
          {
               break;
          }

          line_count = temp_line_count;
     }

     return std::make_pair(line_count, it);
}

Ini::Mutable_Iterator
Ini::find_mutable_section_start(uint64_t index)
{
     auto it = _sections.begin();

     for (int i = 0; i < index; ++i)
     {
          if (it == _sections.end())
               break;
          else
               ++it;
     }

     return it;
}

std::pair<uint64_t, Ini::Mutable_Iterator>
Ini::find_mutable_section_start(std::string_view name)
{
     int line_count = 0;
     auto it = _sections.begin();

     for (; it != _sections.end(); ++it)
     {
          uint64_t temp_line_count = line_count + it->Line_Count();

          if (it->Name() == name)
          {
               break;
          }

          line_count = temp_line_count;
     }

     return std::make_pair(line_count, it);
}

void Ini::Add_Value_To_Section(std::string_view section_name, uint64_t line_number, std::string_view value)
{
     auto [_, it] = find_mutable_section_start(section_name);

     if (it == _sections.end())
     {
          Add_Section(section_name);
          auto [_, it] = find_mutable_section_start(section_name);
     }

     if (!section_name.empty() && line_number != kMax_Line_Number)
          ++line_number;

     it->Add_Value(line_number, value);
}
