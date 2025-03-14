#include "ini.h"

#include <testie.h>

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

using Ini_Format::Ini;
using Ini_Format::Section;

UNIT_TEST(Ini_Tests)

BEGIN_TEST_GROUP(Ini)
     DECLARE_TEST(Default_Constructor)
     DECLARE_TEST(From_File)
     DECLARE_TEST(From_Buffer)
     DECLARE_TEST(Add_Value)
     DECLARE_TEST(Add_Section)
     DECLARE_TEST(Add_Value_To_Section)
END_TEST_GROUP(Ini)

DEFINE_TEST(Ini, Default_Constructor)
{
     Ini ini;

     VERIFY_ARE_EQUAL(uint64_t{0}, ini.Line_Count());
     VERIFY_ARE_EQUAL(size_t{0}, ini.Lines().size());

     return true;
}

DEFINE_TEST(Ini, From_File)
{
     auto ini = Ini::From_File("sample.ini");

     VERIFY_ARE_EQUAL(uint64_t{11}, ini.Line_Count());

     VERIFY_ARE_EQUAL("[Device10K1]"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("VEN_1102&DEV_0011"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("VEN_1102&DEV_0013"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL(""sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("[Ctversion]"sv, ini.Find_Line(4));
     VERIFY_ARE_EQUAL("GROUP=AUDIO"sv, ini.Find_Line(5));
     VERIFY_ARE_EQUAL("Ver=1.05.0058"sv, ini.Find_Line(6));
     VERIFY_ARE_EQUAL(""sv, ini.Find_Line(7));
     VERIFY_ARE_EQUAL(";; Vista Section *********************************************************"sv, ini.Find_Line(8));
     VERIFY_ARE_EQUAL("[INFInstall.W2KWDM.6]"sv, ini.Find_Line(9));
     VERIFY_ARE_EQUAL("CtxHda.inf"sv, ini.Find_Line(10));

     return true;
}

DEFINE_TEST(Ini, From_Buffer)
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

     auto ini = Ini::From_Buffer(ini_buffer);

     VERIFY_ARE_EQUAL(uint64_t{11}, ini.Line_Count());

     VERIFY_ARE_EQUAL("[Device10K1]"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("VEN_1102&DEV_0011"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("VEN_1102&DEV_0013"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL(""sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("[Ctversion]"sv, ini.Find_Line(4));
     VERIFY_ARE_EQUAL("GROUP=AUDIO"sv, ini.Find_Line(5));
     VERIFY_ARE_EQUAL("Ver=1.05.0058"sv, ini.Find_Line(6));
     VERIFY_ARE_EQUAL(""sv, ini.Find_Line(7));
     VERIFY_ARE_EQUAL(";; Vista Section *********************************************************"sv, ini.Find_Line(8));
     VERIFY_ARE_EQUAL("[INFInstall.W2KWDM.6]"sv, ini.Find_Line(9));
     VERIFY_ARE_EQUAL("CtxHda.inf"sv, ini.Find_Line(10));

     VERIFY_IS_FALSE(ini.Find_Value("GROUP"));
     VERIFY_IS_TRUE(ini.Find_Value("Ctversion", "GROUP"));

     std::string value;
     VERIFY_IS_TRUE(ini.Find_Value("Ctversion", "GROUP", &value));
     VERIFY_ARE_EQUAL("AUDIO"s, value);

     VERIFY_IS_TRUE(ini.Find_Value("INFInstall.W2KWDM.6", "CtxHda.inf", &value));
     VERIFY(value.empty());

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

     VERIFY(ini.Add_Section("section_1"));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Line_Count());
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL(uint64_t{0}, ini.Find_Section("section_1"));

     ini.Add_Value("value_1");
     VERIFY_ARE_EQUAL(uint64_t{2}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Find_Section("section_1"));

     VERIFY(ini.Add_Section("section_2"));
     VERIFY_ARE_EQUAL(uint64_t{3}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("[section_2]"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Find_Section("section_1"));
     VERIFY_ARE_EQUAL(uint64_t{2}, ini.Find_Section("section_2"));

     ini.Add_Value_To_Section("section_2", "value_2");
     VERIFY_ARE_EQUAL(uint64_t{4}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("[section_2]"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("value_2"sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Find_Section("section_1"));
     VERIFY_ARE_EQUAL(uint64_t{2}, ini.Find_Section("section_2"));

     VERIFY_IS_FALSE(ini.Add_Section(0, "section_3"));
     VERIFY_IS_TRUE(ini.Add_Section(1, "section_3"));
     VERIFY_ARE_EQUAL(uint64_t{5}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("[section_3]"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("[section_2]"sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("value_2"sv, ini.Find_Line(4));
     VERIFY_ARE_EQUAL(uint64_t{2}, ini.Find_Section("section_1"));
     VERIFY_ARE_EQUAL(uint64_t{3}, ini.Find_Section("section_2"));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Find_Section("section_3"));

     ini.Add_Value_To_Section("section_3", "value_3");
     VERIFY_ARE_EQUAL(uint64_t{6}, ini.Line_Count());
     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("[section_3]"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("value_3"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("[section_2]"sv, ini.Find_Line(4));
     VERIFY_ARE_EQUAL("value_2"sv, ini.Find_Line(5));
     VERIFY_ARE_EQUAL(uint64_t{3}, ini.Find_Section("section_1"));
     VERIFY_ARE_EQUAL(uint64_t{4}, ini.Find_Section("section_2"));
     VERIFY_ARE_EQUAL(uint64_t{1}, ini.Find_Section("section_3"));

     return true;
}

DEFINE_TEST(Ini, Add_Value_To_Section)
{
     Ini ini;

     ini.Add_Value("value_1");
     ini.Add_Section("section_1");
     ini.Add_Value_To_Section("section_1", "value_2");
     ini.Add_Value_To_Section("section_1", "name_1", "value_3");
     ini.Add_Value_To_Section("section_1", 0, "name_2", "value_4");
     ini.Add_Value_To_Section("section_1", 1500, "name_3", "value_5");

     VERIFY_ARE_EQUAL("value_1"sv, ini.Find_Line(0));
     VERIFY_ARE_EQUAL("[section_1]"sv, ini.Find_Line(1));
     VERIFY_ARE_EQUAL("name_2=value_4"sv, ini.Find_Line(2));
     VERIFY_ARE_EQUAL("value_2"sv, ini.Find_Line(3));
     VERIFY_ARE_EQUAL("name_1=value_3"sv, ini.Find_Line(4));
     VERIFY_ARE_EQUAL("name_3=value_5"sv, ini.Find_Line(5));

     return true;
}
