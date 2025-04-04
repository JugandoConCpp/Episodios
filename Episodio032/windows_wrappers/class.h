#ifndef WINDOWS_WRAPPERS__CLASS_H__INCLUDED
#define WINDOWS_WRAPPERS__CLASS_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"

namespace Windows
{
     typedef LRESULT CALLBACK (*Window_Proc) (
              HWND window_handle,
              UINT message,
              WPARAM w_param,
              LPARAM l_param);

     class Window;

     class Class
     {
          private:
               std::string _name;
               WNDCLASSEX _class;
               LPCSTR _pointer;
               ATOM _atom;

               inline static LRESULT CALLBACK _default_window_proc(
                        HWND window_handle,
                        UINT message,
                        WPARAM w_param,
                        LPARAM l_param);

          public:
               inline static Window* Get_Window(HWND window_handle)
               { return (Window*) GetWindowLongPtr(window_handle, 0); }

               inline Class(std::string_view class_name);
               inline Class(HINSTANCE instance, std::string_view class_name);
               inline Class(Class&& klass);

               Class(Class const&) = delete;

               ~Class() { Unregister(); }

               std::string const& Name() const { return _name; }

               operator ATOM() const { return _atom; }
               operator LPCSTR() const { return _pointer; }

               WNDCLASSEX& Params() { return _class; }
               WNDCLASSEX const& Params() const { return _class; }

               inline HRESULT Register();
               inline HRESULT Unregister();

               inline HRESULT Set_Window_Proc(Window_Proc new_window_proc);
               inline HRESULT Set_Hinstance(HINSTANCE new_hinstance);
               inline HRESULT Set_Window_Extra(int extra);
     };  // class Class

     namespace Builtin_Classes
     {
          inline static Class EDIT("EDIT");
     }  // namespace Builtin_Classes

     LRESULT CALLBACK Class::_default_window_proc(
              HWND window_handle,
              UINT message,
              WPARAM w_param,
              LPARAM l_param)
     {
          LRESULT return_value = 0;

          switch (message)
          {
               case WM_CLOSE:
                    PostQuitMessage(0x13);
                    break;

               default:
                    return_value = DefWindowProc(window_handle, message, w_param, l_param);
          }

          return return_value;
     }

     Class::Class(std::string_view class_name):
          _name{class_name},
          _class{},
          _pointer{_name.c_str()},
          _atom{}
     {}

     Class::Class(HINSTANCE instance, std::string_view class_name):
          _name{class_name},
          _class {
               .cbSize = sizeof(WNDCLASSEX),
               .style = CS_DROPSHADOW,
               .lpfnWndProc = &_default_window_proc,
               .cbClsExtra = 0,
               .cbWndExtra = sizeof(Window*),
               .hInstance = instance,
               .hIcon = nullptr,
               .hCursor = nullptr,
               .hbrBackground = nullptr,
               .lpszMenuName = nullptr,
               .lpszClassName = _name.c_str(),
               .hIconSm = nullptr
          },
          _pointer{},
          _atom{}
     {}

     Class::Class(Class&& klass):
          _name(std::move(klass._name)),
          _class(klass._class),
          _pointer(klass._pointer),
          _atom(klass._atom)
     {
          klass._pointer = nullptr;
          klass._atom = 0;
     }

     HRESULT Class::Register()
     {
          if (_atom)
               return S_FALSE;

          _atom = RegisterClassEx(&Params());
          _pointer = MAKEINTATOM(_atom);

          return IF_FAILS_GET_LAST_ERROR(_atom);
     }

     HRESULT Class::Unregister()
     {
          if (!_atom)
               return S_FALSE;

          BOOL bool_result = UnregisterClass(*this, _class.hInstance);

          if (bool_result)
          {
               _atom = 0;
          }

          return IF_FAILS_GET_LAST_ERROR(bool_result);
     }

     HRESULT Class::Set_Window_Proc(Window_Proc new_window_proc)
     {
          if (_pointer || _atom)
               return E_INVALIDARG;

          _class.lpfnWndProc = new_window_proc;
          return S_OK;
     }

     HRESULT Class::Set_Hinstance(HINSTANCE new_hinstance)
     {
          if (_pointer || _atom)
               return E_INVALIDARG;

          _class.hInstance = new_hinstance;
          return S_OK;
     }

     HRESULT Class::Set_Window_Extra(int extra)
     {
          if (_pointer || _atom)
               return E_INVALIDARG;

          _class.cbWndExtra = extra;
          return S_OK;
     }
}  // namespace Windows

#endif  // WINDOWS_WRAPPERS__CLASS_H__INCLUDED
