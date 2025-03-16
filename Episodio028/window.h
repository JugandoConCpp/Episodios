#ifndef WINDOW_H__INCLUDED
#define WINDOW_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"
#include "class.h"

class Window
{
     private:
          std::string _title;
          HWND _handle;
          HRESULT _creation_result;

     public:
          Window(HINSTANCE instance, Class klass, std::string_view title):
               _title{title},
               _handle{},
               _creation_result{S_OK}
          {
               _handle = CreateWindow(
                              klass.Name().c_str(), _title.c_str(), WS_TILEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                              nullptr, nullptr, instance, nullptr);

               _creation_result = (_handle == nullptr)? HRESULT_FROM_WIN32(GetLastError()): S_OK;
          }

          operator bool() const { return SUCCEEDED(_creation_result); }
          bool operator!() const { return !static_cast<bool>(*this); }

          operator HWND() const { return _handle; }

          HRESULT Creation_Result() const { return _creation_result; }

          void Show() const
          {
               ShowWindow(*this, SW_NORMAL);
          }

          void Update() const
          {
               UpdateWindow(*this);
          }

          HRESULT Destroy() const
          {
               return IF_FAILS_GET_LAST_ERROR(DestroyWindow(*this));
          }

          void Run_Main_Loop() const
          {
               bool quit = false;

               do {
                    MSG message;
                    BOOL keep_going = GetMessage(&message, *this, 0, 0);
                    quit = !keep_going;

                    if (keep_going == -1)
                    {
                         Sleep(100);
                         continue;
                    }

                    TranslateMessage(&message);
                    DispatchMessage(&message);
               } while (!quit);
          }
};  // class Window

#endif  // WINDOW_H__INCLUDED
