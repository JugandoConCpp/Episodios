#ifndef INI__LINE_H__INCLUDED
#define INI__LINE_H__INCLUDED

#include <format>
#include <string>
#include <string_view>

#ifndef INI_FORMAT_API
#define INI_FORMAT_API __declspec(dllimport)
#endif  // INI_FORMAT_API

namespace Ini_Format
{
     class Line
     {
          private:
               std::string _line;

          public:
               Line() = default;
               Line(Line const&) = default;
               Line(Line&&) = default;

               Line(char const* data):
                    _line{data}
               {}

               Line(std::string_view data):
                    _line{data}
               {}

               Line(std::string&& data):
                    _line{data}
               {}

               operator std::string_view() const
               { return _line; }

               operator std::string const&() const
               { return _line; }

               static Line Make_Section_Header(std::string_view name)
               { return Line{std::format("[{}]", name)}; }

               INI_FORMAT_API
               bool Is_Section_Header(std::string* section_name = nullptr) const;

               static Line Make_KVP(std::string_view name, std::string_view value)
               { return Line{std::format("{}={}", name, value)}; }

               INI_FORMAT_API
               bool Is_KVP(std::string* name = nullptr, std::string* value = nullptr);
     };  // class Line

}  // namespace Ini_Format


#endif  // INI__LINE_H__INCLUDED
