#ifndef INI_H__INCLUDED
#define INI_H__INCLUDED

#include <cstdint>
#include <format>
#include <list>
#include <string>
#include <string_view>

class Ini
{
     private:
          std::list<std::string> _lines;

     protected:
          bool line_is_section(std::string const& line) const
          {
               return ((line.size() > 2) && (line.front() == '[') && (line.back() == ']'));
          }

          std::list<std::string>::const_iterator find_line_impl(int line_number) const
          {
               uint64_t current_line_number = 0;
               std::list<std::string>::const_iterator it;

               for (it = _lines.begin(); it != _lines.end(); ++it)
               {
                    if (current_line_number == line_number)
                         return it;

                    if (line_is_section(*it))
                         return it;

                    ++current_line_number;
               }

               return it;
          }

     public:
          Ini(){}

          std::list<std::string> const& Lines() const
          { return _lines; }

          uint64_t Line_Count() const
          { return uint64_t(_lines.size()); }

          //
          // Encuentra la línea con el número especificado dentro de la sección global.
          //
          std::string_view Find_Line(int line_number) const
          {
               auto it = find_line_impl(line_number);

               return (it == _lines.end())? std::string_view{""}: *it;
          }

          //
          // Agrega una nueva línea a la sección global en el índice señalado por el parámetro
          // line_number.  Si el número excede el número de líneas en la sección, se agregará después de
          // la última línea.
          //
          void Add_Value(uint64_t line_number, std::string_view value)
          { _lines.insert(find_line_impl(line_number), std::string{value}); }

          void Add_Value(std::string_view value)
          { Add_Value(std::numeric_limits<uint64_t>::max(), value); }

          void Add_Value(uint64_t line_number, std::string_view name, std::string_view value)
          { Add_Value(line_number, std::format("{}={}", name, value)); }

          void Add_Value(std::string_view name, std::string_view value)
          { Add_Value(std::numeric_limits<uint64_t>::max(), name, value); }
};  // class Ini

#endif  // INI_H__INCLUDED
