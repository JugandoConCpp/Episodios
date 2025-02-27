#ifndef TRONCO_H__INCLUDED
#define TRONCO_H__INCLUDED

#include <format>
#include <print>
#include <string>
#include <string_view>

#include <Windows.h>

namespace Tronco
{
     enum class Log_Level
     {
          Verbose,
          Information,
          Warning,
          Error,
          Critical
     };

     inline std::string_view Get_Log_Level_Name(Log_Level level)
     {
          switch(level)
          {
               case Log_Level::Verbose:      return "Verbose";
               case Log_Level::Information:  return "Information";
               case Log_Level::Warning:      return "Warning";
               case Log_Level::Error:        return "Error";
               case Log_Level::Critical:     return "Critical";
          }

          return "Unknown_Level";
     }

     class Log_Entry
     {
          private:
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
               Log_Entry(Log_Level level, std::string_view filename, int line_number):
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

               ~Log_Entry()
               {
                    std::println(
                         "[{}/{:02}/{:02} {:02}:{:02}:{:02}.{:03}] [{}] [{}] [{}:{}]",
                         _timestamp.wYear, _timestamp.wMonth, _timestamp.wDay,
                         _timestamp.wHour, _timestamp.wMinute, _timestamp.wSecond, _timestamp.wMilliseconds,
                         Get_Log_Level_Name(_level), _message, _filename, _line_number);
               }
     };
}  // namespace Tronco

#define LOG(Level)       Tronco::Log_Entry(Level, __FILE__, __LINE__)

#define LOG_VERBOSE      LOG(Tronco::Log_Level::Verbose)
#define LOG_INFO         LOG(Tronco::Log_Level::Information)
#define LOG_WARNING      LOG(Tronco::Log_Level::Warning)
#define LOG_ERROR        LOG(Tronco::Log_Level::Error)
#define LOG_CRITICAL     LOG(Tronco::Log_Level::Critical)

#define LOG_FMT(Level, fmt, ...)     Tronco::Log_Entry(Level, __FILE__, __LINE__).Format_Message(fmt, __VA_ARGS__)

#define LOG_VERBOSE_FMT(fmt, ...)       LOG_FMT(Tronco::Log_Level::Verbose, fmt, __VA_ARGS__)
#define LOG_INFO_FMT(fmt, ...)          LOG_FMT(Tronco::Log_Level::Information, fmt, __VA_ARGS__)
#define LOG_WARNING_FMT(fmt, ...)       LOG_FMT(Tronco::Log_Level::Warning, fmt, __VA_ARGS__)
#define LOG_ERROR_FMT(fmt, ...)         LOG_FMT(Tronco::Log_Level::Error, fmt, __VA_ARGS__)
#define LOG_CRITICAL_FMT(fmt, ...)      LOG_FMT(Tronco::Log_Level::Critical, fmt, __VA_ARGS__)

#endif  // TRONCO_H__INCLUDED
