#ifndef TRONCO__MACROS_H__INCLUDED
#define TRONCO__MACROS_H__INCLUDED

#ifndef TRONCO_API
#define TRONCO_API __declspec(dllimport)
#endif  // TRONCO_API

#define LOG_TO(Logger, Level)       ::Tronco::Log_Entry(Logger, Level, __FILE__, __LINE__)

#define LOG(Level) LOG_TO(::Tronco::Tronco::Select_Logger(), Level)

#define LOG_VERBOSE      LOG(::Tronco::Log_Level::Verbose)
#define LOG_INFO         LOG(::Tronco::Log_Level::Information)
#define LOG_WARNING      LOG(::Tronco::Log_Level::Warning)
#define LOG_ERROR        LOG(::Tronco::Log_Level::Error)
#define LOG_CRITICAL     LOG(::Tronco::Log_Level::Critical)

#define LOG_FMT_TO(Logger, Level, fmt, ...)        ::Tronco::Log_Entry(Logger, Level, __FILE__, __LINE__).Format_Message(fmt, __VA_ARGS__)

#define LOG_FMT(Level, fmt, ...)        LOG_FMT_TO(::Tronco::Tronco::Select_Logger(), Level, fmt, __VA_ARGS__)

#define LOG_VERBOSE_FMT(fmt, ...)       LOG_FMT(::Tronco::Log_Level::Verbose, fmt, __VA_ARGS__)
#define LOG_INFO_FMT(fmt, ...)          LOG_FMT(::Tronco::Log_Level::Information, fmt, __VA_ARGS__)
#define LOG_WARNING_FMT(fmt, ...)       LOG_FMT(::Tronco::Log_Level::Warning, fmt, __VA_ARGS__)
#define LOG_ERROR_FMT(fmt, ...)         LOG_FMT(::Tronco::Log_Level::Error, fmt, __VA_ARGS__)
#define LOG_CRITICAL_FMT(fmt, ...)      LOG_FMT(::Tronco::Log_Level::Critical, fmt, __VA_ARGS__)

#endif  // TRONCO__MACROS_H__INCLUDED
