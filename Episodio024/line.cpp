#define INI_FORMAT_API __declspec(dllexport)

#include "line.h"

#include <string>

using Ini_Format::Line;

bool Line::Is_Section_Header(std::string* section_name) const
{
     bool is_section_header =
          ((_line.size() > 2) && (_line.front() == '[') && (_line.back() == ']'));

     if (section_name)
     {
          if (is_section_header)
          {
               *section_name = _line.substr(1, _line.length() - 2);
          } else {
               section_name->clear();
          }
     }

     return is_section_header;
}

bool Line::Is_KVP(std::string* name, std::string* value)
{
     std::string::size_type pos = _line.find('=');
     bool is_kvp = (pos != std::string::npos);

     if (is_kvp)
     {
          if (name)
          {
               *name = _line.substr(0, pos);
          }

          if (value)
          {
               *value = _line.substr(pos + 1);
          }
     } else {
          if (name)
          {
               name->clear();
          }

          if (value)
          {
               value->clear();
          }
     }

     return is_kvp;
}

