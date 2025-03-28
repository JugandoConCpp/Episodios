#ifndef WINDOWS_WRAPPERS__MENU_H__INCLUDED
#define WINDOWS_WRAPPERS__MENU_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"

namespace Windows
{
     class Menu
     {
          private:
               HMENU _menu;
               Window* _parent;
               std::map<HMENU, std::string> _popups;

          public:
               Menu():
                    _menu{CreateMenu()},
                    _parent{}
               {}

               operator HMENU() const { return _menu; }

               ~Menu()
               {
                    if (!_parent)
                    {
                         DestroyMenu(*this);
                    }
               }

               void Set_Parent(Window* parent)
               {
                    _parent = parent;
               }

               inline void Add_Popup(std::string const& name, std::list<std::string> const& items);

               std::string_view Get_Popup_Name(HMENU handle) const
               {
                    auto it = _popups.find(handle);
                    return (it != _popups.end())? it->second: std::string_view("");
               }
     };

     void Menu::Add_Popup(std::string const& name, std::list<std::string> const& items)
     {
          HMENU popup = CreateMenu();
          _popups[popup] = name;
          int sequence = 0;

          for (auto const& item: items)
          {
               if (item.empty())
               {
                    AppendMenu(popup, MF_SEPARATOR, ++sequence, item.c_str());
               } else {
                    AppendMenu(popup, MF_ENABLED | MF_STRING | MF_UNCHECKED, ++sequence, item.c_str());
               }
          }

          AppendMenu(*this, MF_POPUP, (UINT_PTR) popup, name.c_str());
     }
}  // namespace Windows

#endif  // WINDOWS_WRAPPERS__MENU_H__INCLUDED
