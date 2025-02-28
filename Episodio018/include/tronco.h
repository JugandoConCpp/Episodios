#ifndef TRONCO_H__INCLUDED
#define TRONCO_H__INCLUDED

#include <algorithm>
#include <format>
#include <list>
#include <memory>
#include <print>
#include <string>
#include <string_view>

#include <Windows.h>

#include "channel.h"
#include "log_level.h"
#include "log_entry.h"

#ifndef TRONCO_API
#define __declspec(dllimport)
#endif  // TRONCO_API

namespace Tronco
{
     class Tronco
     {
          private:
               std::list<std::unique_ptr<Channel>> _channels;

          public:
               TRONCO_API static Tronco* Default_Logger;

               inline static Tronco& Select_Logger()
               {
                    static Tronco Dummy_Logger;

                    return
                         ::Tronco::Tronco::Default_Logger?
                              *::Tronco::Tronco::Default_Logger:
                              Dummy_Logger;
               }

               Tronco()
               {
                    if (!Default_Logger)
                         Default_Logger = this;
               }

               void Add_Channel(std::unique_ptr<Channel> new_channel)
               {
                    _channels.emplace_back(std::move(new_channel));
               }

               TRONCO_API
               void Write_Log_Entry(Log_Entry const& entry) const;
     };
}  // namespace Tronco

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

#endif  // TRONCO_H__INCLUDED
