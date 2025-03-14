#define INI_FORMAT_API __declspec(dllexport)

#include "line.h"

#include <functional>
#include <cctype>
#include <string>

using Ini_Format::Line;

bool Line::Is_Section_Header(std::string_view raw_line, std::string* section_name)
{
     bool is_section_header =
          ((raw_line.size() > 2) && (raw_line.front() == '[') && (raw_line.back() == ']'));

     if (section_name)
     {
          if (is_section_header)
          {
               *section_name = raw_line.substr(1, raw_line.length() - 2);
          } else {
               section_name->clear();
          }
     }

     return is_section_header;
}

namespace {
     bool is_not_space(int ch)
     {
          return !std::isspace(ch);
     }

     bool is_not_quote_mark(int ch)
     {
          return ch != '"';
     }

     inline void ltrim(std::string &string, std::function<bool(int)> op)
     {
         string.erase(
               string.begin(),
               std::find_if(string.begin(), string.end(), op));
     }

     // trim from end (in place)
     inline void rtrim(std::string &string, std::function<bool(int)> op)
     {
         string.erase(
               std::find_if(string.rbegin(), string.rend(), op).base(),
               string.end());
     }

     inline void trim(std::string& string, std::function<bool(int)> op = &is_not_space)
     {
          ltrim(string, op);
          rtrim(string, op);
     }
}  // namespace

bool Line::Is_KVP(std::string* name, std::string* value) const
{
     std::string::size_type pos = _line.find('=');
     bool is_kvp = (pos != std::string::npos);

     if (is_kvp)
     {
          if (name)
          {
               *name = _line.substr(0, pos);
               trim(*name, &is_not_quote_mark);
               trim(*name);
               trim(*name, &is_not_quote_mark);
          }

          if (value)
          {
               *value = _line.substr(pos + 1);
               trim(*value, &is_not_quote_mark);
               trim(*value);
               trim(*value, &is_not_quote_mark);
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

