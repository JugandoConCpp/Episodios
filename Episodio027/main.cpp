#include <print>

#include <Windows.h>

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_command)
{
     WNDCLASSEX class_params
     {
          .cbSize = sizeof(WNDCLASSEX),
          .style = CS_DROPSHADOW,
          .lpfnWndProc = &DefWindowProcW,
          .cbClsExtra = 0,
          .cbWndExtra = 0,
          .hInstance = instance,
          .hIcon = nullptr,
          .hCursor = nullptr,
          .hbrBackground = nullptr,
          .lpszMenuName = nullptr,
          .lpszClassName = "AppClass",
          .hIconSm = nullptr
     };

     ATOM klass = RegisterClassEx(&class_params);

     if (klass == 0)
     {
          DWORD error = GetLastError();
          std::println("Could not register class: {}", error);
          return 1;
     }

     HWND window_handle = CreateWindow("AppClass", "This is the window title", WS_TILEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, instance, nullptr);

     if (window_handle == 0)
     {
          DWORD error = GetLastError();
          std::println("Could not create window: {}", error);
          return 2;
     }

     ShowWindow(window_handle, SW_NORMAL);
     UpdateWindow(window_handle);

     Sleep(1000);

     BOOL bool_result = DestroyWindow(window_handle);

     if (bool_result == 0)
     {
          DWORD error = GetLastError();
          std::println("Could not destroy window: {}", error);
          return 255;
     }

     bool_result = UnregisterClass(MAKEINTATOM(klass), instance);

     if (bool_result == 0)
     {
          DWORD error = GetLastError();
          std::println("Could not unregister class: {}", error);
          return 255;
     }

     return 0;
}
