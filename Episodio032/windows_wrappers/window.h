#ifndef WINDOWS_WRAPPERS__WINDOW_H__INCLUDED
#define WINDOWS_WRAPPERS__WINDOW_H__INCLUDED

#include <string>
#include <string_view>

#define NOMINMAX
#include <Windows.h>

#include "common.h"
#include "class.h"

namespace Windows
{
     class Window
     {
          private:
               Class& _class;
               std::string _title;
               HWND _handle;
               Window* _parent;

               mutable bool _main_loop_running;

          protected:
               inline virtual HWND create_impl(Location location, Size size);

          public:
               inline Window(
                         Class& klass,
                         std::string_view title,
                         Window* parent);

               Window(Class& klass, std::string_view title):
                    Window(klass, title, nullptr)
               {}

               virtual ~Window() { Destroy(); }

               operator HWND() const { return Handle(); }
               HWND Handle() const { return _handle; }

               HWND Parent_Handle() const { return _parent? _parent->Handle(): nullptr; }

               Class& Klass() const { return _class; }

               std::string const& Title() const { return _title; }

               inline virtual HRESULT Create(HINSTANCE instance, Location location, Size size);

               virtual void Show()
               { ShowWindow(*this, SW_NORMAL); }

               virtual void Update()
               { UpdateWindow(*this); }

               inline virtual HRESULT Destroy();
               inline virtual void Run_Main_Loop() const;
               inline virtual void Stop_Main_Loop() const;

               virtual bool Message_Hook(MSG const& message) const
               {
                    return true;
               }

               virtual bool Translate_Accelerator(MSG& message) const
               {
                    return true;
               }
     };  // class Window

     HWND Window::create_impl(Location location, Size size)
     {
          HWND handle = CreateWindow(
                         Klass().Name().c_str(), _title.c_str(), WS_TILEDWINDOW, location.x, location.y, size.x, size.y,
                         Parent_Handle(), nullptr, Klass().Params().hInstance, nullptr);
          return handle;
     }

     Window::Window(
               Class& klass,
               std::string_view title,
               Window* parent):
          _class{klass},
          _title{title},
          _handle{},
          _parent{parent},
          _main_loop_running{false}
     {}

     HRESULT Window::Create(HINSTANCE /*instance*/, Location location, Size size)
     {
          _handle = create_impl(location, size);

          HRESULT result = (_handle == nullptr)? HRESULT_FROM_WIN32(GetLastError()): S_OK;

          if (SUCCEEDED(result))
               SetWindowLongPtr(_handle, 0, LONG_PTR(this));

          return result;
     }

     HRESULT Window::Destroy()
     {
          if (!_handle)
               return S_FALSE;

          BOOL bool_result = DestroyWindow(*this);

          if (bool_result)
               _handle = nullptr;

          return IF_FAILS_GET_LAST_ERROR(bool_result);
     }

     void Window::Run_Main_Loop() const
     {
          _main_loop_running = true;

          do {
               MSG message;
               BOOL result = GetMessage(&message, *this, 0, 0);

               if (result == -1)
               {
                    HRESULT last_error = HRESULT_FROM_WIN32(GetLastError());
                    Sleep(100);
                    continue;
               }

               _main_loop_running = (result == TRUE);

               if (Translate_Accelerator(message))
                    continue;

               bool dispatch_message = Message_Hook(message);

               if (dispatch_message)
               {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
               }
          } while (_main_loop_running);
     }

     void Window::Stop_Main_Loop() const
     {
          _main_loop_running = false;
     }
}  // namespace Windows

#endif  // WINDOWS_WRAPPERS__WINDOW_H__INCLUDED
