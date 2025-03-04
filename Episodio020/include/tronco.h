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
#include "console_channel.h"
#include "file_channel.h"
#include "log_level.h"
#include "log_entry.h"
#include "macros.h"

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

               template<typename Channel_Type, typename... Args>
               void Add_Channel(Args const&... args)
               {
                    _channels.emplace_back(std::make_unique<Channel_Type>(args...));
               }

               TRONCO_API
               void Write_Log_Entry(Log_Entry const& entry) const;
     };
}  // namespace Tronco

#endif  // TRONCO_H__INCLUDED
