#include "ini.h"

#include <testie.h>

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

UNIT_TEST(Ini_Tests)

BEGIN_TEST_GROUP(Ini)
     DECLARE_TEST(Default_Constructor)
     DECLARE_TEST(Add_Value)
     DECLARE_TEST(Add_Section)
END_TEST_GROUP(Ini)

DEFINE_TEST(Ini, Default_Constructor)
{
     Ini ini;

     VERIFY_ARE_EQUAL(uint64_t{0}, ini.Line_Count());
     VERIFY_ARE_EQUAL(size_t{0}, ini.Lines().size());

     return true;
}

DEFINE_TEST(Ini, Add_Value)
{
     Ini ini;

     ini.Add_Value("value_1");
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));

     ini.Add_Value("name_2", "value_2");
     VERIFY_ARE_EQUAL(uint64_t{2}, ini.Line_Count());
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(1));

     ini.Add_Value(0, "value_1");
     VERIFY_ARE_EQUAL(uint64_t{3}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(2));

     ini.Add_Value(0, "name_2", "value_2");
     VERIFY_ARE_EQUAL(uint64_t{4}, ini.Line_Count());
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(3));

     ini.Add_Value(2, "value_1");
     VERIFY_ARE_EQUAL(uint64_t{5}, ini.Line_Count());
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(4));

     ini.Add_Value(2, "name_7", "value_7");
     VERIFY_ARE_EQUAL(uint64_t{6}, ini.Line_Count());
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("name_7=value_7"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(4));
     VERIFY_ARE_EQUAL("name_2=value_2"sv, ini.Find_Line(5));

     return true;
}

DEFINE_TEST(Ini, Add_Section)
{
     Ini ini;

     ini.Add_Section("section_1");
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Line_Count());
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL(uint64_t{0}, ini.Find_Section("section_1"));

     ini.Add_Value("value_1");
     VERIFY_ARE_EQUAL(uint64_t{2}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Find_Section("section_1"));

     return true;
}
