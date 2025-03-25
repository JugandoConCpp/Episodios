#include <print>

#define NOMINMAX
#include <Windows.h>

#include "common.h"

#include "class.h"
#include "main_window.h"


#include <print>

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_command)
{
     Main_Window_Class klass(nullptr);
     EXIT_ON_ERROR(klass.Register());

     Main_Window main_window(nullptr, klass, command_line);
     EXIT_ON_ERROR(main_window.Create());

     main_window.Show();
     main_window.Update();
     main_window.Run_Main_Loop();

     return 0;
}
