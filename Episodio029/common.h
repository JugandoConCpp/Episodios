#ifndef COMMON_H__INCLUDED
#define COMMON_H__INCLUDED

#include <string>
#include <string_view>

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

#define IF_FAILS_GET_LAST_ERROR(bool_result)                                    \
     (((bool_result) == 0)? HRESULT_FROM_WIN32(GetLastError()): S_OK)           \

template<UINT Type = MB_OK, typename... Args>
inline void Create_Message_Box(
          char const* title,
          std::format_string<Args&...> const& format_string,
          Args&&... args)
{
     std::string message = std::format(format_string, args...);
     MessageBox(nullptr, message.c_str(), title, Type);
}

#define EXIT_ON_ERROR(operation)                                                               \
     {                                                                                         \
          HRESULT result = (operation);                                                        \
          if (FAILED(result))                                                                  \
          {                                                                                    \
               Create_Message_Box<MB_OK>(                                                      \
                    "Error!",                                                                  \
                    "Operation failed [result=0x{:08x}]: {}",                                  \
                    uint32_t(result),                                                          \
                    #operation);                                                               \
               return result;                                                                  \
          }                                                                                    \
     }                                                                                         \

#endif  // COMMON_H__INCLUDED
