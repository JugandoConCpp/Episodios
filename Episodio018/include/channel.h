#ifndef TRONCO__CHANNEL_H__INCLUDED
#define TRONCO__CHANNEL_H__INCLUDED

#include <print>
#include <string>

#include "log_entry.h"

namespace Tronco
{
     class Channel
     {
          private:
               std::string _name;

          protected:
               Channel(std::string const& name):
                    _name{name}
               {}

          public:
               virtual ~Channel(){}

               virtual void Open() = 0;
               virtual void Close() = 0;
               virtual void Write_Log_Entry(Log_Entry const& entry) const = 0;
     };

     class Console_Channel: public Channel
     {
          public:
               Console_Channel(std::string const& name):
                    Channel{name}
               {}

               void Open() override {}
               void Close() override {}

               void Write_Log_Entry(Log_Entry const& entry) const override
               {
                    std::println("{}", entry.Format_Log_Entry());
               }
     };
}  // namespace Tronco

#endif  // TRONCO__CHANNEL_H__INCLUDED
