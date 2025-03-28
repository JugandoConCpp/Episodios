#ifndef WINDOWS_WRAPPERS__COMMON_H__INCLUDED
#define WINDOWS_WRAPPERS__COMMON_H__INCLUDED

#include <format>
#include <string>
#include <string_view>

#define IF_FAILS_GET_LAST_ERROR(bool_result)                                    \
     (((bool_result) == 0)? HRESULT_FROM_WIN32(GetLastError()): S_OK)           \

#define EXIT_ON_ERROR(operation)                                                               \
     {                                                                                         \
          HRESULT result = (operation);                                                        \
          if (FAILED(result))                                                                  \
          {                                                                                    \
               ::Windows::Create_Message_Box<MB_OK>(                                           \
                    "Error!",                                                                  \
                    "Operation failed [result=0x{:08x}]: {}",                                  \
                    uint32_t(result),                                                          \
                    #operation);                                                               \
               return result;                                                                  \
          }                                                                                    \
     }                                                                                         \

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

namespace Windows
{
     template<UINT Type = MB_OK, typename... Args>
     inline void Create_Message_Box(
               char const* title,
               std::format_string<Args&...> const& format_string,
               Args&&... args)
     {
          std::string message = std::format(format_string, args...);
          MessageBox(nullptr, message.c_str(), title, Type);
     }

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
}  // Windows

#endif  // WINDOWS_WRAPPERS__COMMON_H__INCLUDED
