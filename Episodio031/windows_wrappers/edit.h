#ifndef WINDOWS_WRAPPERS__EDIT_H__INCLUDED
#define WINDOWS_WRAPPERS__EDIT_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"

namespace Windows
{
     class Edit: public Window
     {
          protected:
               HWND create_impl(Location location, Size size) override
               {
                    return CreateWindow(
                                   Klass().Name().c_str(), Title().c_str(), WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_BORDER | ES_MULTILINE,
                                   GetX(), GetY(), size.x, size.y,
                                   Parent_Handle(), nullptr, Klass().Params().hInstance, nullptr);
               }

          public:
               Edit(Window* parent):
                    Window(Windows::Builtin_Classes::EDIT, std::string_view{}, parent)
               {}

               constexpr Location GetLocation() const
               {
                    return Location(10, 10);
               }

               constexpr int GetX() const
               {
                    return GetLocation().x;
               }

               constexpr int GetY() const
               {
                    return GetLocation().y;
               }

               std::string Get_Text() const
               {
                    int length = GetWindowTextLength(*this);
                    std::string string(length, '\0');
                    GetWindowText(*this, &string[0], length);
                    return string;
               }

               void Set_Text(std::string const& contents) const
               {
                    SetWindowText(*this, &contents[0]);
               }
     };  // class Edit
}  // namespace Windows

#endif  // WINDOWS_WRAPPERS__EDIT_H__INCLUDED
