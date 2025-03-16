#ifndef COMMON_H__INCLUDED
#define COMMON_H__INCLUDED

#include <print>
#include <string>
#include <string_view>

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

#define IF_FAILS_GET_LAST_ERROR(bool_result)                                    \
     (((bool_result) == 0)? HRESULT_FROM_WIN32(GetLastError()): S_OK)           \

#define EXIT_ON_ERROR(operation)                                                               \
     {                                                                                         \
          HRESULT result = (operation);                                                        \
          if (FAILED(result))                                                                  \
          {                                                                                    \
               std::println("Operation failed [result=0x{:08x}: {}", result, #operation);      \
               return result;                                                                  \
          }                                                                                    \
     }                                                                                         \

#endif  // COMMON_H__INCLUDED
