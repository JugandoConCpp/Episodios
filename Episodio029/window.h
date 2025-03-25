#ifndef WINDOW_H__INCLUDED
#define WINDOW_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

#include "common.h"
#include "class.h"

struct Location
{
     int x, y;

     constexpr Location(int x, int y): x{x}, y{x} {}
};

struct Size
{
     int x, y;

     constexpr Size(int x, int y): x{x}, y{x} {}
};

static inline constexpr Location Default_Location(CW_USEDEFAULT, CW_USEDEFAULT);
static inline constexpr Size Default_Size(CW_USEDEFAULT, CW_USEDEFAULT);

class Window
{
     private:
          Class& _class;
          std::string _title;
          HWND _handle;
          Window* _parent;

     protected:
          virtual HWND create_impl(Location location, Size size)
          {
               HWND handle = CreateWindow(
                              Klass().Name().c_str(), _title.c_str(), WS_TILEDWINDOW, location.x, location.y, size.x, size.y,
                              Parent_Handle(), nullptr, Klass().Params().hInstance, nullptr);
               return handle;
          }

     public:
          Window(
                    HINSTANCE instance,
                    Class& klass,
                    std::string_view title,
                    Window* parent):
               _class{klass},
               _title{title},
               _handle{},
               _parent{parent}
          {}

          Window(HINSTANCE instance, Class& klass, std::string_view title):
               Window(instance, klass, title, nullptr)
          {}

          virtual ~Window() { Destroy(); }

          operator HWND() const { return Handle(); }
          HWND Handle() const { return _handle; }

          HWND Parent_Handle() const { return _parent? _parent->Handle(): nullptr; }

          Class& Klass() const { return _class; }

          std::string const& Title() const { return _title; }

          virtual HRESULT Create(Location location, Size size)
          {
               _handle = create_impl(location, size);

               HRESULT result = (_handle == nullptr)? HRESULT_FROM_WIN32(GetLastError()): S_OK;

               if (SUCCEEDED(result))
                    SetWindowLongPtr(_handle, 0, LONG_PTR(this));

               return result;
          }

          virtual void Show()
          {
               ShowWindow(*this, SW_NORMAL);
          }

          virtual void Update()
          {
               UpdateWindow(*this);
          }

          virtual HRESULT Destroy()
          {
               if (!_handle)
                    return S_FALSE;

               BOOL bool_result = DestroyWindow(*this);

               if (bool_result)
                    _handle = nullptr;

               return IF_FAILS_GET_LAST_ERROR(bool_result);
          }

          virtual void Run_Main_Loop() const
          {
               bool quit = false;

               do {
                    MSG message;
                    BOOL keep_going = GetMessage(&message, *this, 0, 0);
                    quit = !keep_going;

                    if (keep_going == -1)
                    {
                         HRESULT last_error = HRESULT_FROM_WIN32(GetLastError());
                         std::println("last_error: 0x{:x}", uint32_t(last_error));
                         Sleep(100);
                         continue;
                    }

                    bool dispatch_message = Message_Hook(message);

                    if (dispatch_message)
                    {
                         TranslateMessage(&message);
                         DispatchMessage(&message);
                    }
               } while (!quit);
          }

          virtual bool Message_Hook(MSG const& message) const
          {
               return true;
          }
};  // class Window

#endif  // WINDOW_H__INCLUDED
