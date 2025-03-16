#ifndef CLASS_H__INCLUDED
#define CLASS_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"

class Class
{
     private:
          std::string _name;
          WNDCLASSEX _class;
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
                         DestroyWindow(window_handle);
                         break;

                    case WM_DESTROY:
                         PostQuitMessage(0);
                         break;

                    default:
                         return_value = DefWindowProc(window_handle, message, w_param, l_param);
               }

               return return_value;
          }

     public:
          Class(HINSTANCE instance, std::string_view class_name):
               _name{class_name},
               _class {
                    .cbSize = sizeof(WNDCLASSEX),
                    .style = CS_DROPSHADOW,
                    .lpfnWndProc = &_default_window_proc,
                    .cbClsExtra = 0,
                    .cbWndExtra = 0,
                    .hInstance = instance,
                    .hIcon = nullptr,
                    .hCursor = nullptr,
                    .hbrBackground = nullptr,
                    .lpszMenuName = nullptr,
                    .lpszClassName = _name.c_str(),
                    .hIconSm = nullptr
               }
          {}

          std::string const& Name() const { return _name; }

          operator ATOM() const { return _atom; }
          operator LPCSTR() const { return MAKEINTATOM(_atom); }

          WNDCLASSEX& Params() { return _class; }
          WNDCLASSEX const& Params() const { return _class; }

          HRESULT Register()
          {
               _atom = RegisterClassEx(&Params());

               return IF_FAILS_GET_LAST_ERROR(_atom);
          }

          HRESULT Unregister()
          {
               return IF_FAILS_GET_LAST_ERROR(UnregisterClass(*this, _class.hInstance));
          }
};  // class Class

#endif  // CLASS_H__INCLUDED
