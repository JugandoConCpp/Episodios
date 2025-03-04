#include "channel.h"

#include <string>

#include <testie.h>

#include "log_entry.h"
#include "log_level.h"
#include "tronco.h"

UNIT_TEST(Channel_Headers)

BEGIN_TEST_GROUP(Channel)
     DECLARE_TEST(Constructor)
     DECLARE_TEST(Write_Log_Entry)
END_TEST_GROUP(Channel)

struct Unexpected_Function_Call{};

DEFINE_TEST(Channel, Constructor)
{
     struct Test_Channel: Tronco::Channel {
          Test_Channel(std::string const& name):
               Tronco::Channel{name}
          {}

          void Open() override { throw Unexpected_Function_Call{}; }
          void Close() override { throw Unexpected_Function_Call{}; }

          void Write_Log_Entry(std::string_view entry) const override
          { throw Unexpected_Function_Call{}; }
     };

     std::string channel_name = "Test channel";
     Test_Channel c(channel_name);

     VERIFY_ARE_EQUAL(channel_name, c.Name());

     channel_name = "Some other random string";
     VERIFY_ARE_NOT_EQUAL(channel_name, c.Name());

     return true;
}

DEFINE_TEST(Channel, Write_Log_Entry)
{
     struct Test_Channel: Tronco::Channel {
          mutable std::vector<std::string> log_entries;

          Test_Channel(std::string const& name):
               Tronco::Channel{name}
          {}

          void Open() override { throw Unexpected_Function_Call{}; }
          void Close() override { throw Unexpected_Function_Call{}; }

          using Tronco::Channel::Write_Log_Entry;
          void Write_Log_Entry(std::string_view entry) const override
          {
               log_entries.emplace_back(entry);
          }
     };

     std::string channel_name = "Test channel";
     Test_Channel c(channel_name);

     std::string unformatted_log_entry = "some log entry";

     c.Write_Log_Entry(unformatted_log_entry);
     VERIFY_ARE_EQUAL(size_t{1}, c.log_entries.size());
     VERIFY_ARE_EQUAL(unformatted_log_entry, c.log_entries[0]);

     Tronco::Tronco::Default_Logger = nullptr;
     Tronco::Log_Entry log_entry(Tronco::Tronco::Select_Logger(), Tronco::Log_Level::Verbose, "filename", 1337);
     log_entry << unformatted_log_entry;

     c.log_entries.clear();
     c.Write_Log_Entry(log_entry);
     VERIFY_ARE_EQUAL(size_t{1}, c.log_entries.size());

     std::string const& logged_string = c.log_entries[0];
     VERIFY_ARE_NOT_EQUAL(unformatted_log_entry, logged_string);

     size_t substring_location = logged_string.find(unformatted_log_entry);
     VERIFY_ARE_NOT_EQUAL(std::string::npos, substring_location);
     VERIFY_ARE_EQUAL(
          unformatted_log_entry,
          logged_string.substr(substring_location, unformatted_log_entry.length()));

     return true;
}

