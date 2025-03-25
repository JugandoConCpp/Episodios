#ifndef MAIN_WINDOW_H__INCLUDED
#define MAIN_WINDOW_H__INCLUDED

#include "window.h"

#include <fstream>

#include <ini.h>

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
          Edit(HINSTANCE instance, Window* parent):
               Window(instance, Builtin_Classes::EDIT, std::string_view{}, parent)
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

using Ini_Format::Ini;

class Main_Window: public Window
{
     private:
          Edit _edit;

          bool _ctrl_key_down;
          std::string _filename;

          std::string _configuration_file_name;
          Ini _configuration;

          void Read_File() const
          {
               if (_filename.empty())
                    return;

               auto text = _edit.Get_Text();

               std::ifstream stream(_filename, std::ios::ate);
               std::string::size_type size = stream.tellg();
               stream.seekg(0);

               std::string string(size, '\0');
               stream.read(&string[0], size);
               _edit.Set_Text(string);
          }

          void Write_File() const
          {
               if (_filename.empty())
                    return;

               auto text = _edit.Get_Text();

               std::ofstream stream(_filename);

               if (!stream)
                    return;

               stream.write(text.c_str(), text.length());
          }

          int Read_Configuration_Value(std::string_view name)
          {
               std::string value;
               _configuration.Find_Value(name, &value);
               return std::stoi(value);
          }

          void Read_Configuration()
          {
               _configuration = Ini::From_File(_configuration_file_name);

               auto y1 = Read_Configuration_Value("top");
               auto y2 = Read_Configuration_Value("bottom");
               auto x1 = Read_Configuration_Value("left");
               auto x2 = Read_Configuration_Value("right");

               SetWindowPos(*this, nullptr, x1, y1, x2-x1, y2-y1, 0);
          }

          void Save_Configuration() const
          {
               std::ofstream stream(_configuration_file_name);

               for (auto const& line: _configuration.Lines())
               {
                    stream << std::string_view(line) << std::endl;
               }
          }


     public:
          inline static LRESULT CALLBACK Window_Proc(
                   HWND window_handle,
                   UINT message,
                   WPARAM w_param,
                   LPARAM l_param);

          Main_Window(HINSTANCE instance, Class& klass, std::string_view filename):
               Window(instance, klass, std::format("Edit: {}", filename)),
               _edit(instance, this),
               _ctrl_key_down{false},
               _filename{filename},
               _configuration_file_name{"edit.ini"},
               _configuration{}
          {}

          ~Main_Window() override
          {
               Save_Configuration();
          }

          HRESULT Create(Location location = Default_Location, Size size = Default_Size) override
          {
               HRESULT result{};

               result = Window::Create(location, size);
               if (FAILED(result)) return result;

               result = _edit.Create(Location(10, 10), Size(500, 500));
               if (FAILED(result)) return result;

               Read_File();
               Read_Configuration();

               return S_OK;
          }

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

          void OnResize()
          {
               RECT rect;
               GetClientRect(*this, &rect);
               int width = rect.right - rect.left - 20;
               int height = rect.bottom - rect.top - 20;

               if (width < 20)
                    width = 20;

               if (height < 20)
                    height = 20;

               SetWindowPos(_edit, nullptr, _edit.GetX(), _edit.GetY(), width, height, 0);

               GetWindowRect(*this, &rect);
               _configuration = Ini();
               _configuration.Add_Value("top", std::format("{}", rect.top));
               _configuration.Add_Value("bottom", std::format("{}", rect.bottom));
               _configuration.Add_Value("left", std::format("{}", rect.left));
               _configuration.Add_Value("right", std::format("{}", rect.right));
          }

          bool Message_Hook(MSG const& message) const override
          {
               bool key_down = (message.message == WM_KEYDOWN);
               bool key_up = (message.message == WM_KEYUP);

               if (key_down || key_up)
               {
                    char key = message.wParam;
                    bool is_control = (key == VK_CONTROL);

                    if (is_control || _ctrl_key_down)
                    {
                         SendMessage(*this, message.message, message.wParam, message.lParam);
                         return false;
                    }
               }

               return true;
          }
};  // class Main_Window

LRESULT CALLBACK Main_Window::Window_Proc(
         HWND window_handle,
         UINT message,
         WPARAM w_param,
         LPARAM l_param)
{
     LRESULT return_value = 0;

     auto window = static_cast<Main_Window*>(Class::Get_Window(window_handle));

     switch (message)
     {
          case WM_KEYDOWN:
               if (w_param == VK_CONTROL)
                    window->_ctrl_key_down = true;

               break;

          case WM_KEYUP:
               switch(w_param)
               {
                    case VK_CONTROL:
                         window->_ctrl_key_down = false;
                         break;

                    case 'S':
                         if (window->_ctrl_key_down)
                              window->Write_File();
               }

               break;

          case WM_PAINT:
               {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(window_handle, &ps);
                    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
                    EndPaint(window_handle, &ps);
                    break;
               }

          case WM_SIZE:
               window->OnResize();
               break;

          case WM_CLOSE:
               PostQuitMessage(0);
               break;

          default:
               return_value = DefWindowProc(window_handle, message, w_param, l_param);
     }

     return return_value;
}

class Main_Window_Class: public Class
{
     public:
          Main_Window_Class(HINSTANCE instance):
               Class(instance, "MainWindowClass")
          {
               if (FAILED(set_window_proc(&Main_Window::Window_Proc)))
               {
                    throw std::string("Failed to update window proc!");
               }
          }
};  // class Main_Window_Class


#endif  // MAIN_WINDOW_H__INCLUDED
