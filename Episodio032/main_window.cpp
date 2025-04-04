#include "main_window.h"

std::unique_ptr<Main_Window_Class> Main_Window::_class = std::make_unique<Main_Window_Class>();

inline constexpr uint16_t IDM_SEPARATOR = 0;
inline constexpr uint16_t IDM_FILE_NEW = 1;
inline constexpr uint16_t IDM_FILE_OPEN = 2;
inline constexpr uint16_t IDM_FILE_SAVE = 3;
inline constexpr uint16_t IDM_FILE_EXIT = 4;

std::vector<ACCEL> KeyboardAcceleratorTable = {
     {FCONTROL, 'N', IDM_FILE_NEW},
     {FCONTROL, 'O', IDM_FILE_OPEN},
     {FCONTROL, 'S', IDM_FILE_SAVE},
     {FALT, VK_F4, IDM_FILE_EXIT}
};

Main_Window_Class::Main_Window_Class():
     Class(nullptr, "Main_Window_Class")
{
}

Main_Window::Main_Window(std::string_view filename):
     Window(*_class, std::format("Edit: {}", filename)),
     _edit(this),
     _menu{},
     _accel{},
     _ctrl_key_down{false},
     _filename{filename},
     _configuration_file_name{"edit.ini"},
     _configuration{}
{}

Main_Window::~Main_Window()
{
     if (_accel)
          DestroyAcceleratorTable(_accel);

     Save_Configuration();
}

void Main_Window::Clear_Buffer()
{
     _edit.Clear_Text();
}

HRESULT Main_Window::Create(HINSTANCE instance, Location location, Size size)
{
     if (Handle())
          return ERROR_ALREADY_EXISTS;

     HRESULT result{};

     _class->Set_Window_Proc(&Window_Proc);
     _class->Set_Hinstance(instance);
     result = _class->Register();
     if (FAILED(result)) return result;

     _menu.Add_Popup(
          "&File", {
          {IDM_FILE_NEW, "&New"},
          {IDM_FILE_OPEN, "&Open"},
          {IDM_FILE_SAVE, "&Save"},
          {IDM_SEPARATOR, ""},
          {IDM_FILE_EXIT, "E&xit"}});

     result = Window::Create(instance, location, size);
     if (FAILED(result)) return result;

     _menu.Set_Parent(this);

     _accel = CreateAcceleratorTable(&KeyboardAcceleratorTable[0], int(KeyboardAcceleratorTable.size()));

     result = _edit.Create(instance, Location(10, 10), Size(500, 500));
     if (FAILED(result)) return result;

     Read_File();
     Read_Configuration();

     return S_OK;
}

void Main_Window::On_Resize()
{
     RECT rect;
     GetClientRect(*this, &rect);
     int width = rect.right - rect.left - 20;
     int height = rect.bottom - rect.top - 20;

     if (width < 20)
          width = 20;

     if (height < 20)
          height = 20;

     SetWindowPos(_edit, nullptr, _edit.GetX(), _edit.GetY(), width, height, 0);

     GetWindowRect(*this, &rect);
     _configuration = Ini();
     _configuration.Add_Value("top", std::format("{}", rect.top));
     _configuration.Add_Value("bottom", std::format("{}", rect.bottom));
     _configuration.Add_Value("left", std::format("{}", rect.left));
     _configuration.Add_Value("right", std::format("{}", rect.right));
}

void Main_Window::On_Command(uint32_t command)
{
     auto const source = HIWORD(command);

     if (source == 1)
          Windows::Create_Message_Box<>("Accelerator", "command: 0x{:x}", command);

     if ((source == 0) || (source == 1))
          return On_Menu_Command(command);
}

void Main_Window::On_Menu_Command(uint16_t menu_command)
{
     switch(menu_command)
     {
          case IDM_FILE_NEW:
               Clear_Buffer();
               break;

          case IDM_FILE_EXIT:
               Destroy();
               Stop_Main_Loop();
               break;
     }
}

bool Main_Window::Message_Hook(MSG const& message) const
{
//     bool key_down = (message.message == WM_KEYDOWN);
//     bool key_up = (message.message == WM_KEYUP);
//
//     if (key_down || key_up)
//     {
//          char key = message.wParam;
//          bool is_control = (key == VK_CONTROL);
//
//          if (is_control || _ctrl_key_down)
//          {
//               SendMessage(*this, message.message, message.wParam, message.lParam);
//               return false;
//          }
//     }

     return true;
}

LRESULT CALLBACK Main_Window::Window_Proc(
          HWND window_handle,
          UINT message,
          WPARAM w_param,
          LPARAM l_param)
{
     LRESULT return_value{};
     bool use_default_window_proc{};

     auto window = static_cast<Main_Window*>(Class::Get_Window(window_handle));

     switch (message)
     {
          case WM_COMMAND:
//               Windows::Create_Message_Box<>("Accelerator", "w_param: 0x{:x}", w_param);
               window->On_Command((uint16_t) w_param);
               break;

          case WM_PAINT:
               {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(window_handle, &ps);
                    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW+1));
                    EndPaint(window_handle, &ps);
                    break;
               }

          case WM_SIZE:
               window->On_Resize();
               break;

          case WM_CLOSE:
               PostQuitMessage(0);
               window->Stop_Main_Loop();
               break;

          default:
               use_default_window_proc = true;
               break;
     }

     if (use_default_window_proc)
          return_value = DefWindowProc(window_handle, message, w_param, l_param);

     return return_value;
}

HWND Main_Window::create_impl(Location location, Size size)
{
     HWND handle = CreateWindow(
                    Klass().Name().c_str(), Title().c_str(), WS_TILEDWINDOW, location.x, location.y, size.x, size.y,
                    Parent_Handle(), _menu, Klass().Params().hInstance, nullptr);
     return handle;
}

void Main_Window::Read_File() const
{
     if (_filename.empty())
          return;

     auto text = _edit.Get_Text();

     std::ifstream stream(_filename, std::ios::ate);
     std::string::size_type size = stream.tellg();
     stream.seekg(0);

     std::string string(size, '\0');
     stream.read(&string[0], size);
     _edit.Set_Text(string);
}

void Main_Window::Write_File() const
{
     if (_filename.empty())
          return;

     auto text = _edit.Get_Text();

     std::ofstream stream(_filename);

     if (!stream)
          return;

     stream.write(text.c_str(), text.length());
}

int Main_Window::Read_Configuration_Value(std::string_view name)
{
     std::string value;
     _configuration.Find_Value(name, &value);
     return std::stoi(value);
}

void Main_Window::Read_Configuration()
{
     _configuration = Ini::From_File(_configuration_file_name);

     auto y1 = Read_Configuration_Value("top");
     auto y2 = Read_Configuration_Value("bottom");
     auto x1 = Read_Configuration_Value("left");
     auto x2 = Read_Configuration_Value("right");

     SetWindowPos(*this, nullptr, x1, y1, x2-x1, y2-y1, 0);
}

void Main_Window::Save_Configuration() const
{
     std::ofstream stream(_configuration_file_name);

     for (auto const& line: _configuration.Lines())
     {
          stream << std::string_view(line) << std::endl;
     }
}
