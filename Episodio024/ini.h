#ifndef INI_H__INCLUDED
#define INI_H__INCLUDED

#include <cstdint>
#include <list>
#include <string>
#include <string_view>

#include "line.h"

#ifndef INI_FORMAT_API
#define INI_FORMAT_API __declspec(dllimport)
#endif  // INI_FORMAT_API

namespace Ini_Format
{
     //
     // El archivo INI consiste en varias secciones, cada una con valores.  Si al inicio del
     // archivo no se declara una sección, se puede conceptualizar como una sección global.
     // Los índices se manejarán desde cero (para la sección global) y se incrementarán para cada
     // sección adicional que se vaya agregando.
     //

     class Ini
     {
          private:
               inline static constexpr uint64_t kMax_Line_Number = std::numeric_limits<uint64_t>::max();
               std::list<Line> _lines;

          protected:
               std::list<Line>::const_iterator
               find_line_impl(uint64_t line_number, bool stop_on_section_header) const
               { return find_line_impl(line_number, _lines.begin(), stop_on_section_header); }

               INI_FORMAT_API
               std::list<Line>::const_iterator
               find_line_impl(uint64_t line_number, std::list<Line>::const_iterator, bool stop_on_section_header) const;

               INI_FORMAT_API
               std::list<Line>::const_iterator
               find_section_start(uint64_t index) const;

               INI_FORMAT_API
               std::pair<uint64_t, std::list<Line>::const_iterator>
               find_section_start(std::string_view name) const;

          public:
               Ini() = default;

               INI_FORMAT_API
               static Ini From_File(std::string const& filename);

               INI_FORMAT_API
               static Ini From_Buffer(std::string_view buffer);

///               void Dump() const
///               {
///                    for (auto const& line: _lines)
///                         std::println("LINE: {}", std::string_view(line));
///               }

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

               //
               // Encuentra la línea con el número especificado en todo el archivo.
               //
               std::string_view Find_Line(uint64_t line_number) const
               {
                    auto it = find_line_impl(line_number, false);

                    return (it == _lines.end())? std::string_view{""}: std::string_view{*it};
               }

               //
               // Encuentra la línea con el número especificado dentro de la sección global.
               //
               std::string_view Find_Line_In_Section(uint64_t line_number) const
               {
                    auto it = find_line_impl(line_number, true);

                    return (it == _lines.end())? std::string_view{""}: std::string_view{*it};
               }

               //
               // Agrega una sección al final, con el nombre especificado.
               //
               // Resultado: true si la operación fue exitosa, false si no.
               //
               bool Add_Section(std::string_view name)
               { return Add_Section(kMax_Line_Number, name); }

               //
               // Agrega una sección en la posición especificada por index, relativa a las
               // demás secciones.
               //
               // Resultado: true si la operación fue exitosa, false si no.
               //
               bool Add_Section(uint64_t index, std::string_view name)
               {
                    if (index == 0)
                    {
                         //
                         // No se puede agregar una sección antes de la sección global (por definición).
                         //

                         return false;
                    }

                    _lines.insert(find_section_start(index), Line::Make_Section_Header(name));
                    return true;
               }

               //
               // Encuentra la sección con el nombre especificado y regresa el número de línea.
               //
               uint64_t Find_Section(std::string_view name)
               {
                    auto [line_number, it] = find_section_start(name);

                    return ((it == _lines.end())? kMax_Line_Number: line_number);
               }

               //
               // Agrega una nueva línea a la sección global en el índice señalado por el parámetro
               // line_number.  Si el número excede el número de líneas en la sección, se agregará después de
               // la última línea.
               //
               void Add_Value(uint64_t line_number, std::string_view value)
               { _lines.insert(find_line_impl(line_number, true), std::string{value}); }

               void Add_Value(std::string_view value)
               { Add_Value(kMax_Line_Number, value); }

               void Add_Value(uint64_t line_number, std::string_view name, std::string_view value)
               { Add_Value(line_number, Line::Make_KVP(name, value)); }

               void Add_Value(std::string_view name, std::string_view value)
               { Add_Value(kMax_Line_Number, name, value); }

               //
               // Agrega una nueva línea a la sección especificada en el índice señalado por el parámetro
               // line_number.  Si el número excede el número de líneas en la sección, se agregará después de
               // la última línea.
               //
               INI_FORMAT_API
               void Add_Value_To_Section(std::string_view section_name, uint64_t line_number, std::string_view value);

               void Add_Value_To_Section(std::string_view section_name, std::string_view value)
               { Add_Value_To_Section(section_name, kMax_Line_Number, value); }

               void Add_Value_To_Section(std::string_view section_name, uint64_t line_number, std::string_view name, std::string_view value)
               { Add_Value_To_Section(section_name, line_number, Line::Make_KVP(name, value)); }

               void Add_Value_To_Section(std::string_view section_name, std::string_view name, std::string_view value)
               { Add_Value_To_Section(section_name, kMax_Line_Number, name, value); }
     };  // class Ini
}  // namespace Ini_Format

#endif  // INI_H__INCLUDED
