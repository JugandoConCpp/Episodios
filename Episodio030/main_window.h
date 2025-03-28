#ifndef MAIN_WINDOW_H__INCLUDED
#define MAIN_WINDOW_H__INCLUDED

#include <window.h>

#include <fstream>
#include <map>
#include <string>
#include <string_view>

#include <ini.h>

#include <class.h>
#include <edit.h>
#include <menu.h>

using Windows::Class;
using Windows::Location;
using Windows::Window;
using Windows::Size;
using Windows::Default_Location;
using Windows::Default_Size;

using Ini_Format::Ini;

struct Main_Window_Class: Windows::Class
{
     Main_Window_Class();
};

class Main_Window: public Windows::Window
{
     private:
          static std::unique_ptr<Main_Window_Class> _class;

          Windows::Edit _edit;
          Windows::Menu _menu;

          bool _ctrl_key_down;
          std::string _filename;

          std::string _configuration_file_name;
          Ini _configuration;

          HWND create_impl(Location location, Size size) override;

          void Read_File() const;
          void Write_File() const;

          int Read_Configuration_Value(std::string_view name);
          void Read_Configuration();
          void Save_Configuration() const;

     public:
          inline static LRESULT CALLBACK Window_Proc(
                   HWND window_handle,
                   UINT message,
                   WPARAM w_param,
                   LPARAM l_param);

          Main_Window(std::string_view filename);
          ~Main_Window() override;

          HRESULT Create(
               HINSTANCE instance,
               Location location = Default_Location,
               Size size = Default_Size) override;

          void Show() override
          {
               Window::Show();
               _edit.Show();
          }

          void Update() override
          {
               Window::Update();
               _edit.Update();
          }

          void On_Resize();
          void Execute_File_Menu_Command(uint16_t menu_item);
          void On_Menu_Command(HMENU menu, uint16_t menu_item);
          bool Message_Hook(MSG const& message) const override;
};  // class Main_Window

#endif  // MAIN_WINDOW_H__INCLUDED
