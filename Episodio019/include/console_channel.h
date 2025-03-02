#ifndef TRONCO__CONSOLE_CHANNEL_H__INCLUDED
#define TRONCO__CONSOLE_CHANNEL_H__INCLUDED

#include "channel.h"

#include <string>

namespace Tronco
{
     class Console_Channel: public Channel
     {
          public:
               Console_Channel(std::string const& name):
                    Channel{name}
               {}

               void Open() override {}
               void Close() override {}

               void Write_Log_Entry(std::string_view entry) const override
               {
                    std::println("{}", entry);
               }
     };
}  // namespace Tronco

#endif  // TRONCO__CONSOLE_CHANNEL_H__INCLUDED
