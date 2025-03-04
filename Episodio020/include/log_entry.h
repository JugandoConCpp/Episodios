#ifndef TRONCO__LOG_ENTRY_H__INCLUDED
#define TRONCO__LOG_ENTRY_H__INCLUDED

#include <format>
#include <string>

#include <Windows.h>

#include "log_level.h"
#include "macros.h"

namespace Tronco
{
     class Tronco;

     class Log_Entry
     {
          private:
               Tronco& _logger;
               Log_Level const _level;
               SYSTEMTIME const _timestamp;
               std::string const _filename;
               int const _line_number;

               std::string _message;

          protected:
               SYSTEMTIME get_system_time() const
               {
                    SYSTEMTIME time;
                    GetSystemTime(&time);
                    return time;
               }

          public:
               Log_Entry(Tronco& logger, Log_Level level, std::string_view filename, int line_number):
                    _logger{logger},
                    _level{level},
                    _timestamp{get_system_time()},
                    _filename{filename},
                    _line_number{line_number},
                    _message{}
               {}

               Log_Entry& operator<<(std::string_view text)
               {
                    _message.append(text);
                    return *this;
               }

               template<typename... Args>
               Log_Entry& Format_Message(std::format_string<Args&...> const fmt, Args... args)
               {
                    _message.append(std::format(fmt, args...));
                    return *this;
               }

               TRONCO_API
               std::string Format_Log_Entry() const;

               TRONCO_API
               ~Log_Entry();
     };
}  // namespace Tronco

#endif  // TRONCO__LOG_ENTRY_H__INCLUDED
