#include <print>

#define NOMINMAX
#include <Windows.h>

#include "class.h"
#include "main_window.h"

int WinMain(HINSTANCE instance, HINSTANCE /*previous_instance*/, LPSTR command_line, int show_command)
{
     Main_Window main_window(command_line);
     EXIT_ON_ERROR(main_window.Create(instance));

     main_window.Show();
     main_window.Update();
     main_window.Run_Main_Loop();

     return 0;
}
