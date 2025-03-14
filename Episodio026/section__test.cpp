#include "section.h"

#include <testie.h>

BEGIN_TEST_GROUP(Section)
     DECLARE_TEST(From_Buffer)
END_TEST_GROUP(Section)

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

using Ini_Format::Section;

DEFINE_TEST(Section, From_Buffer)
{
     std::string ini_buffer =
          "[Device10K1]\r\n"
          "VEN_1102&DEV_0011\r\n"
          "VEN_1102&DEV_0013\r\n"
          "\r\n"
          "[Ctversion]\r\n"
          "GROUP=AUDIO\r\n"
          "Ver=1.05.0058\r\n"
          "\r\n"
          ";; Vista Section *********************************************************\r\n"
          "[INFInstall.W2KWDM.6]\r\n"
          "CtxHda.inf";

     auto [section, _] = Section::From_Buffer(ini_buffer);
     VERIFY_ARE_EQUAL(uint64_t{4}, section.Line_Count());
     VERIFY_ARE_EQUAL("[Device10K1]"sv, section.Find_Line(0));
     VERIFY_ARE_EQUAL("VEN_1102&DEV_0011"sv, section.Find_Line(1));
     VERIFY_ARE_EQUAL("VEN_1102&DEV_0013"sv, section.Find_Line(2));
     VERIFY_ARE_EQUAL(""sv, section.Find_Line(3));

     return true;
}
