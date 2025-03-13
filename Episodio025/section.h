#ifndef INI_FORMAT__SECTION__H__INCLUDED
#define INI_FORMAT__SECTION__H__INCLUDED

#include <cstdint>
#include <list>
#include <string>
#include <string_view>
#include <utility>

#include "line.h"

namespace Ini_Format
{
     class Section
     {
          public:
               using Iterator = std::list<Line>::const_iterator;

          private:
               std::list<Line> _lines;
               std::string _name;

          public:
               Section::Iterator End() const { return _lines.end(); }

               INI_FORMAT_API
               Iterator
               find_line_impl(uint64_t line_number) const;

               Section() = default;

               Section(std::string_view name):
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

               void Update_Name()
               {
                    if (_lines.empty())
                         return;

                    _lines.front().Is_Section_Header(&_name);
               }

               std::string_view Name() const { return _name; }

               INI_FORMAT_API
               static
               std::pair<Section, pos_t>
               From_Buffer(std::string_view buffer);

               std::list<Line> const& Lines() const
               { return _lines; }

               uint64_t Line_Count() const
               { return uint64_t(_lines.size()); }

               //
               // Agrega una línea al final del archivo.
               //
               void Add_Line(std::string_view line)
               {
                    _lines.emplace_back(line);
               }

               void Add_Line(Iterator it, std::string_view line)
               {
                    _lines.emplace(it, line);
               }

               //
               // Encuentra la línea con el número especificado en todo el archivo.
               //
               std::string_view Find_Line(uint64_t line_number) const
               {
                    auto it = find_line_impl(line_number);

                    return (it == _lines.end())? std::string_view{""}: std::string_view{*it};
               }

               //
               // Agrega una nueva línea en el índice señalado por el parámetro line_number.
               // Si el número excede el número de líneas en la sección, se agregará después de
               // la última línea.
               //
               void Add_Value(uint64_t line_number, std::string_view value)
               {
                    auto it = find_line_impl(line_number);
                    Add_Line(it, value);
               }

               void Add_Value(std::string_view value)
               { Add_Value(kMax_Line_Number, value); }

               void Add_Value(uint64_t line_number, std::string_view name, std::string_view value)
               { Add_Value(line_number, Line::Make_KVP(name, value)); }

               void Add_Value(std::string_view name, std::string_view value)
               { Add_Value(kMax_Line_Number, name, value); }
     };  // class Section
}  // namespace Ini_Format

#endif  // INI_FORMAT__SECTION__H__INCLUDED
