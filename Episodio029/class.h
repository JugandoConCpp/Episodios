#ifndef CLASS_H__INCLUDED
#define CLASS_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"


#include <print>

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

          static LRESULT CALLBACK _default_window_proc(
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

     protected:
          HRESULT set_window_proc(Window_Proc new_window_proc)
          {
               if (_pointer || _atom)
                    return E_INVALIDARG;

               _class.lpfnWndProc = new_window_proc;
               return S_OK;
          }

          void set_window_extra(int extra)
          {
               _class.cbWndExtra = extra;
          }

     public:
          static Window* Get_Window(HWND window_handle)
          {
               return (Window*) GetWindowLongPtr(window_handle, 0);
          }

          Class(std::string_view class_name):
               _name{class_name},
               _class{},
               _pointer{_name.c_str()},
               _atom{}
          {}

          Class(HINSTANCE instance, std::string_view class_name):
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

          Class(Class&& klass):
               _name(std::move(klass._name)),
               _class(klass._class),
               _pointer(klass._pointer),
               _atom(klass._atom)
          {
               klass._pointer = nullptr;
               klass._atom = 0;
          }

          Class(Class const&) = delete;

          ~Class() { Unregister(); }

          std::string const& Name() const { return _name; }

          operator ATOM() const { return _atom; }
          operator LPCSTR() const { return _pointer; }

          WNDCLASSEX& Params() { return _class; }
          WNDCLASSEX const& Params() const { return _class; }

          HRESULT Register()
          {
               _atom = RegisterClassEx(&Params());
               _pointer = MAKEINTATOM(_atom);

               return IF_FAILS_GET_LAST_ERROR(_atom);
          }

          HRESULT Unregister()
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
};  // class Class

namespace Builtin_Classes
{
     inline static Class EDIT("EDIT");
}

#endif  // CLASS_H__INCLUDED
