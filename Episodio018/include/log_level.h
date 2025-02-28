#ifndef TRONCO__LOG_LEVEL_H__INCLUDED
#define TRONCO__LOG_LEVEL_H__INCLUDED

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
}  // namespace Tronco

#endif  // TRONCO__LOG_LEVEL_H__INCLUDED
