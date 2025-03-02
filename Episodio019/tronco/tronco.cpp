#include <tronco.h>
 
#include <fstream>

TRONCO_API
Tronco::Tronco* Tronco::Tronco::Default_Logger = nullptr;

TRONCO_API
void Tronco::Tronco::Write_Log_Entry(Log_Entry const& entry) const
{
     std::string const formatted_entry = entry.Format_Log_Entry();

     std::for_each(
          _channels.begin(),
          _channels.end(),
          [&formatted_entry](auto& channel)
          {
               channel->Write_Log_Entry(formatted_entry);
          });
}

TRONCO_API
std::string Tronco::Log_Entry::Format_Log_Entry() const
{
     return std::format(
          "[{}/{:02}/{:02} {:02}:{:02}:{:02}.{:03}] [{}] [{}] [{}:{}]",
          _timestamp.wYear, _timestamp.wMonth, _timestamp.wDay,
          _timestamp.wHour, _timestamp.wMinute, _timestamp.wSecond, _timestamp.wMilliseconds,
          Get_Log_Level_Name(_level), _message, _filename, _line_number);
}

TRONCO_API
Tronco::Log_Entry::~Log_Entry()
{
     _logger.Write_Log_Entry(*this);
}
