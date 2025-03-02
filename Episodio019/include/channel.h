#ifndef TRONCO__CHANNEL_H__INCLUDED
#define TRONCO__CHANNEL_H__INCLUDED

#include <string>
#include <string_view>

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

               std::string const& Name() { return _name; }

               virtual void Open() = 0;
               virtual void Close() = 0;
               virtual void Write_Log_Entry(std::string_view entry) const = 0;

               virtual void Write_Log_Entry(Log_Entry const& entry) const
               {
                    Write_Log_Entry(entry.Format_Log_Entry());
               }
     };
}  // namespace Tronco

#endif  // TRONCO__CHANNEL_H__INCLUDED
