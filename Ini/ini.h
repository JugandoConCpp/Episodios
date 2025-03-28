#ifndef INI_H__INCLUDED
#define INI_H__INCLUDED

#include <algorithm>
#include <cstdint>
#include <functional>
#include <list>
#include <numeric>
#include <string>
#include <string_view>

#include "line.h"
#include "section.h"

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
               using Iterator = std::list<Section>::const_iterator;
               using Mutable_Iterator = std::list<Section>::iterator;

               std::list<Section> _sections;

          public:
               Ini()
               {
                    Add_Section("");
               }

               Ini(Ini&& that) = default;

               Ini& operator=(Ini&& that) = default;

               Section::Iterator Line_Iterator_End() const { return _sections.back().End(); }

               INI_FORMAT_API
               std::pair<Section*, Section::Iterator>
               find_line_impl(uint64_t line_number, bool stop_on_section_start);

               std::pair<Section const*, Section::Iterator>
               find_line_impl(uint64_t line_number, bool stop_on_section_start) const
               {
                    auto [section, it] = const_cast<Ini*>(this)->find_line_impl(line_number, stop_on_section_start);

                    return std::make_pair(const_cast<Section const*>(section), it);
               }

               INI_FORMAT_API
               Iterator
               find_section_start(uint64_t index) const;
 
               INI_FORMAT_API
               std::pair<uint64_t, Iterator>
               find_section_start(std::string_view name) const;

               INI_FORMAT_API
               Mutable_Iterator
               find_mutable_section_start(uint64_t index);
 
               INI_FORMAT_API
               std::pair<uint64_t, Mutable_Iterator>
               find_mutable_section_start(std::string_view name);

               INI_FORMAT_API
               static Ini From_File(std::string const& filename);

               INI_FORMAT_API
               static Ini From_Buffer(std::string_view buffer);

               std::list<Line> Lines() const
               {
                    std::list<Line> result;

                    std::for_each(
                         _sections.begin(), _sections.end(),
                         [&result](auto const& section){ result.append_range(section.Lines()); });

                    return result;
               }

               uint64_t Line_Count() const
               {
                    uint64_t count = 0;

                    std::for_each(
                         _sections.begin(), _sections.end(),
                         [&count](auto const& section) { count+= section.Line_Count(); });

                    return count;
               }

               //
               // Agrega una línea al final del archivo.
               //
               void Add_Line(std::string_view raw_line)
               {
                    Line line = raw_line;

                    if (line.Is_Section_Header())
                         Add_Section(line);
                    else
                         _sections.back().Add_Line(line);
               }

               //
               // Encuentra la línea con el número especificado en todo el archivo.
               //
               std::string_view Find_Line(uint64_t line_number) const
               {
                    auto [_, it] = find_line_impl(line_number, false);

                    return (it == Line_Iterator_End())? std::string_view{""}: std::string_view{*it};
               }

               //
               // Encuentra la línea con el número especificado dentro de la sección global.
               //
               std::string_view Find_Line_In_Section(uint64_t line_number) const
               {
                    auto [_, it] = find_line_impl(line_number, true);

                    return (it == Line_Iterator_End())? std::string_view{""}: std::string_view{*it};
               }

               //
               // Agrega una sección al final, con el nombre especificado.
               //
               // Resultado: true si la operación fue exitosa, false si no.
               //
               bool Add_Section(std::string_view name)
               { return Add_Section(kMax_Line_Number, name); }

               //
               // Agrega una sección al final.
               //
               void Add_Section(Section&& section)
               {
                    if (section.Name().empty())
                         _sections.front() = std::move(section);
                    else
                         _sections.emplace_back(section);
               }

               //
               // Agrega una sección en la posición especificada por index, relativa a las
               // demás secciones.
               //
               // Resultado: true si la operación fue exitosa, false si no.
               //
               INI_FORMAT_API
               bool Add_Section(uint64_t index, std::string_view name);

               //
               // Encuentra la sección con el nombre especificado y regresa el número de línea.
               //
               INI_FORMAT_API
               uint64_t Find_Section(std::string_view name);

               //
               // Agrega una nueva línea a la sección global en el índice señalado por el parámetro
               // line_number.  Si el número excede el número de líneas en la sección, se agregará después de
               // la última línea.
               //
               void Add_Value(uint64_t line_number, std::string_view value)
               {
                    auto [section, it] = find_line_impl(line_number, true);
                    section->Add_Line(it, value);
               }

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

               //
               // Las siguientes funciones buscan y extraen un valor especificado.
               //
               INI_FORMAT_API
               bool Find_Value(Section const& section, std::string_view value_name, std::string* value = nullptr) const;

               INI_FORMAT_API
               bool Find_Value(std::string_view section_name, std::string_view value_name, std::string* value = nullptr) const;

               bool Find_Value(std::string_view value_name, std::string* value = nullptr) const
               {
                    return Find_Value(_sections.front(), value_name, value);
               }
     };  // class Ini
}  // namespace Ini_Format

#endif  // INI_H__INCLUDED
