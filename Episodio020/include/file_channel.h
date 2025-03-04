#ifndef TRONCO__FILE_CHANNEL_H__INCLUDED
#define TRONCO__FILE_CHANNEL_H__INCLUDED

#include "channel.h"

#include <fstream>
#include <string>

namespace Tronco
{
     class File_Channel: public Channel
     {
          private:
               mutable std::ofstream _stream;

          public:
               File_Channel(std::string const& name):
                    Channel{name}
               {
                    // TODO: Throw exception on error.
                    Open();
               }

               ~File_Channel() override
               {}

               void Open() override
               {
                    std::string filename = Name();
                    std::string suffix = ".log";


                    if ((filename.length() < suffix.length()) ||
                        (filename.substr(filename.length() - suffix.length()) != suffix))
                    {
                         filename.append(suffix);
                    }

                    _stream.open(filename, std::ios::app);
               }

               void Close() override
               {
                    _stream.close();
               }

               void Write_Log_Entry(std::string_view entry) const override
               {
                    _stream << entry << std::endl;
               }
     };
}  // namespace Tronco

#endif  // TRONCO__FILE_CHANNEL_H__INCLUDED
