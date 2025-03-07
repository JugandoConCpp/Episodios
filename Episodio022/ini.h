#ifndef INI_H__INCLUDED
#define INI_H__INCLUDED

#include <cstdint>
#include <format>
#include <list>
#include <string>
#include <string_view>
#include <utility>

class Ini
{
     public:
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

                    bool Is_Section_Header(std::string* section_name = nullptr) const
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

                    static Line Make_Section_Header(std::string_view name)
                    { return Line{std::format("[{}]", name)}; }

                    static Line Make_KVP(std::string_view name, std::string_view value)
                    { return Line{std::format("{}={}", name, value)}; }
          };

     private:
          inline static constexpr uint64_t kMax_Line_Number = std::numeric_limits<uint64_t>::max();
          std::list<Line> _lines;

     protected:
          std::list<Line>::const_iterator find_line_impl(int line_number) const
          {
               uint64_t current_line_number = 0;
               std::list<Line>::const_iterator it;

               for (it = _lines.begin(); it != _lines.end(); ++it)
               {
                    if (current_line_number == line_number)
                         return it;

                    if (it->Is_Section_Header())
                         return it;

                    ++current_line_number;
               }

               return it;
          }

          std::list<Line>::const_iterator find_section_start(int index) const
          {
               uint64_t current_section_index = -1;
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

          std::pair<uint64_t, std::list<Line>::const_iterator> find_section_start(std::string_view name) const
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
          }

     public:
          Ini() = default;

          std::list<Line> const& Lines() const
          { return _lines; }

          uint64_t Line_Count() const
          { return uint64_t(_lines.size()); }

          //
          // Encuentra la línea con el número especificado dentro de la sección global.
          //
          std::string_view Find_Line(int line_number) const
          {
               auto it = find_line_impl(line_number);

               return (it == _lines.end())? std::string_view{""}: std::string_view{*it};
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
          { _lines.insert(find_line_impl(line_number), std::string{value}); }

          void Add_Value(std::string_view value)
          { Add_Value(kMax_Line_Number, value); }

          void Add_Value(uint64_t line_number, std::string_view name, std::string_view value)
          { Add_Value(line_number, Line::Make_KVP(name, value)); }

          void Add_Value(std::string_view name, std::string_view value)
          { Add_Value(kMax_Line_Number, name, value); }

          void Add_Section(uint64_t index, std::string_view name)
          { _lines.insert(find_section_start(index), Line::Make_Section_Header(name)); }

          void Add_Section(std::string_view name)
          { Add_Section(kMax_Line_Number, name); }
};  // class Ini

#endif  // INI_H__INCLUDED
