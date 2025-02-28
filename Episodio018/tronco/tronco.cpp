#include "tronco.h"

TRONCO_API
Tronco::Tronco* Tronco::Tronco::Default_Logger = nullptr;

TRONCO_API
void Tronco::Tronco::Write_Log_Entry(Log_Entry const& entry) const
{
     std::for_each(
          _channels.begin(),
          _channels.end(),
          [&entry](auto& channel){
               channel->Write_Log_Entry(entry);
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
