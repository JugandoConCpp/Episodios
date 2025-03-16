#include <print>

#include <Windows.h>

#include "common.h"

#include "class.h"
#include "window.h"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_command)
{
     Class klass(nullptr, "AppClass"sv);

     EXIT_ON_ERROR(klass.Register());

     Window main_window(instance, klass, "This is the window title"sv);

     if (!main_window)
     {
          std::println("Could not create window: {}", main_window.Creation_Result());
          return 2;
     }

     main_window.Show();
     main_window.Update();
     main_window.Run_Main_Loop();

     EXIT_ON_ERROR(main_window.Destroy());
     EXIT_ON_ERROR(klass.Unregister());

     return 0;
}
