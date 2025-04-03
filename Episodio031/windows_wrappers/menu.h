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
               struct Item_Descriptor
               {
                    uint16_t Command;
                    std::string Name;

                    bool Is_Separator() const { return Name.empty(); }
               };

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

               inline void Add_Popup(std::string const& name, std::list<Item_Descriptor> const& items);

               std::string_view Get_Popup_Name(HMENU handle) const
               {
                    auto it = _popups.find(handle);
                    return (it != _popups.end())? it->second: std::string_view("");
               }
     };

     void Menu::Add_Popup(std::string const& name, std::list<Item_Descriptor> const& items)
     {
          HMENU popup = CreateMenu();
          _popups[popup] = name;

          for (auto const& item: items)
          {
               if (item.Is_Separator())
               {
                    AppendMenu(popup, MF_SEPARATOR, item.Command, item.Name.c_str());
               } else {
                    AppendMenu(popup, MF_ENABLED | MF_STRING | MF_UNCHECKED, item.Command, item.Name.c_str());
               }
          }

          AppendMenu(*this, MF_POPUP, (UINT_PTR) popup, name.c_str());
     }
}  // namespace Windows

#endif  // WINDOWS_WRAPPERS__MENU_H__INCLUDED
