#ifndef LIBRARY_H__INCLUDED
#define LIBRARY_H__INCLUDED

#include <string>
#include <string_view>

#include <Windows.h>

class Library
{
     private:
          std::string _name;
          HMODULE _handle;
          HRESULT _last_error;

     public:
          Library(std::string_view name):
               _name{name},
               _handle{LoadLibrary(_name.c_str())},
               _last_error{S_OK}
          {
               if (!_handle)
               {
                    _last_error = HRESULT_FROM_WIN32(GetLastError());

                    if (SUCCEEDED(_last_error))
                    {
                         _last_error = ERROR_INVALID_HANDLE;
                    }
               }
          }

          std::string_view Name() const { return _name; }
          HMODULE Handle() const { return _handle; }
          HRESULT Last_Error() const { return _last_error; }

          template<typename Function_Type>
          Function_Type Get_Function(std::string const& function_name)
          {
               return reinterpret_cast<Function_Type>(GetProcAddress(_handle, function_name.c_str()));
          }

          template<typename Return_Type, typename... Args>
          HRESULT Call_Function(std::string const& function_name, Return_Type& return_value, Args... args)
          {
               typedef Return_Type (*Function_Type) (Args...);
               auto function = Get_Function<Function_Type>(function_name);

               if (!function)
               {
                    _last_error = HRESULT_FROM_WIN32(GetLastError());

                    if (SUCCEEDED(_last_error))
                         _last_error = ERROR_INVALID_FUNCTION;

                    return _last_error;
               }

               return_value = (*function)(args...);
               return S_OK;
          }

          ~Library()
          {
               if (_handle)
               {
                    FreeLibrary(_handle);
               }
          }
};  // class Library

#endif  // LIBRARY_H__INCLUDED
