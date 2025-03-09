#include "ini.h"

#include <testie.h>

using std::literals::string_literals::operator""s;
using std::literals::string_view_literals::operator""sv;

using Ini_Format::Line;

BEGIN_TEST_GROUP(Line)
     DECLARE_TEST(Default_Constructor)
     DECLARE_TEST(Copy_Constructor)
     DECLARE_TEST(Move_Constructor)
     DECLARE_TEST(Char_Array_Constructor)
     DECLARE_TEST(String_View_Constructor)
     DECLARE_TEST(String_Move_Constructor)
     DECLARE_TEST(String_Copy_Constructor)
     DECLARE_TEST(Is_Section_Header)
     DECLARE_TEST(Is_KVP)
END_TEST_GROUP(Line)

DEFINE_TEST(Line, Default_Constructor)
{
     Line line;
     VERIFY_ARE_EQUAL(""s, std::string{line});

     return true;
}

DEFINE_TEST(Line, Copy_Constructor)
{
     std::string contents = "oyieufrglhakwef";
     Line line(contents);
     Line copy = line;
     VERIFY_ARE_EQUAL(contents, std::string{line});

     return true;
}

DEFINE_TEST(Line, Move_Constructor)
{
     std::string contents = "ygouiljnvw";
     Line line = Line{contents};
     VERIFY_ARE_EQUAL(contents, std::string{line});

     return true;
}

DEFINE_TEST(Line, Char_Array_Constructor)
{
     char const* contents = "bnyfhlvkajen";
     VERIFY_ARE_EQUAL(std::string_view{contents}, std::string_view{Line{contents}});

     return true;
}

DEFINE_TEST(Line, String_View_Constructor)
{
     auto contents = "bnyfhlvkajen"sv;
     VERIFY_ARE_EQUAL(contents, std::string_view{Line{contents}});

     return true;
}

DEFINE_TEST(Line, String_Move_Constructor)
{
     auto line = Line{"byuhilrcjkaeda"s};
     VERIFY_ARE_EQUAL("byuhilrcjkaeda"s, std::string{line});

     return true;
}

DEFINE_TEST(Line, String_Copy_Constructor)
{
     std::string contents = "ygouiljnvw"s;
     auto line = Line{contents};
     VERIFY_ARE_EQUAL(contents, std::string{line});

     return true;
}

DEFINE_TEST(Line, Is_Section_Header)
{
     std::string* null_name{};
     std::string section_name;

     Line line_1{"lfdkjasadsf"};
     VERIFY_ARE_EQUAL(false, line_1.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_1.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_1.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_2{"lfdkja[sapsf"};
     VERIFY_ARE_EQUAL(false, line_2.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_2.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_2.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_3{"lfdkja[sap]sf"};
     VERIFY_ARE_EQUAL(false, line_3.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_3.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_3.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_4{"lfdkjasadsf"};
     VERIFY_ARE_EQUAL(false, line_4.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_4.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_4.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_5{"lfdkja[sapsf"};
     VERIFY_ARE_EQUAL(false, line_5.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_5.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_5.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_6{"[lfdkja[sap]sf]"};
     VERIFY_ARE_EQUAL(true, line_6.Is_Section_Header());
     VERIFY_ARE_EQUAL(true, line_6.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(true, line_6.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL("lfdkja[sap]sf"s, section_name);

     Line line_7{"[lfdkja[sap]sf] "};
     VERIFY_ARE_EQUAL(false, line_7.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_7.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_7.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_8{" [lfdkja[sap]sf]"};
     VERIFY_ARE_EQUAL(false, line_8.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_8.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_8.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_9{" [lfdkja[sap]sf] "};
     VERIFY_ARE_EQUAL(false, line_9.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_9.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_9.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_A{"[]"};
     VERIFY_ARE_EQUAL(false, line_A.Is_Section_Header());
     VERIFY_ARE_EQUAL(false, line_A.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(false, line_A.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL(""s, section_name);

     Line line_B{"[x]"};
     VERIFY_ARE_EQUAL(true, line_B.Is_Section_Header());
     VERIFY_ARE_EQUAL(true, line_B.Is_Section_Header(null_name));
     VERIFY_ARE_EQUAL(true, line_B.Is_Section_Header(&section_name));
     VERIFY_ARE_EQUAL("x"s, section_name);

     return true;
}

DEFINE_TEST(Line, Is_KVP)
{
     std::string* null_name{};
     std::string* null_value{};
     std::string name{"uhbasdruj"}, value{"ugfhbkga"};

     Line line_1{"iduhfjkbasdj"};
     VERIFY_ARE_EQUAL(false, line_1.Is_KVP());
     VERIFY_ARE_EQUAL(false, line_1.Is_KVP(null_name));
     VERIFY_ARE_EQUAL(false, line_1.Is_KVP(null_name, null_value));
     VERIFY_ARE_EQUAL(false, line_1.Is_KVP(&name, &value));
     VERIFY_ARE_EQUAL(""s, name);
     VERIFY_ARE_EQUAL(""s, value);

     value = "vniwerqewr"s;
     VERIFY_ARE_EQUAL(false, line_1.Is_KVP(null_name, &value));
     VERIFY_ARE_EQUAL(""s, name);
     VERIFY_ARE_EQUAL(""s, value);

     name = "vyhbwsair"s;
     VERIFY_ARE_EQUAL(false, line_1.Is_KVP(&name, null_value));
     VERIFY_ARE_EQUAL(""s, name);
     VERIFY_ARE_EQUAL(""s, value);

     Line line_2{"lfdkja=sapsf"};
     VERIFY_ARE_EQUAL(true, line_2.Is_KVP());
     VERIFY_ARE_EQUAL(true, line_2.Is_KVP(null_name, null_value));
     VERIFY_ARE_EQUAL(true, line_2.Is_KVP(&name, &value));
     VERIFY_ARE_EQUAL("lfdkja"s, name);
     VERIFY_ARE_EQUAL("sapsf"s, value);

     name = "asduyhfalsd"s;
     VERIFY_ARE_EQUAL(true, line_2.Is_KVP(&name));
     VERIFY_ARE_EQUAL("lfdkja"s, name);

     value = "ablskcd"s;
     VERIFY_ARE_EQUAL(true, line_2.Is_KVP(null_name, &value));
     VERIFY_ARE_EQUAL("sapsf"s, value);

     name = "bfvhlk"s;
     VERIFY_ARE_EQUAL(true, line_2.Is_KVP(&name, null_value));
     VERIFY_ARE_EQUAL("lfdkja"s, name);

     Line line_3{"=quewyigfrq"};
     VERIFY_ARE_EQUAL(true, line_3.Is_KVP());
     VERIFY_ARE_EQUAL(true, line_3.Is_KVP(&name, &value));
     VERIFY_IS_TRUE(name.empty());
     VERIFY_ARE_EQUAL("quewyigfrq"s, value);

     Line line_4{"vcidflaerw="};
     VERIFY_ARE_EQUAL(true, line_4.Is_KVP());
     VERIFY_ARE_EQUAL(true, line_4.Is_KVP(&name, &value));
     VERIFY_ARE_EQUAL("vcidflaerw"s, name);
     VERIFY_IS_TRUE(value.empty());

     Line line_5{"vcidflaerw=   "};
     VERIFY_ARE_EQUAL(true, line_5.Is_KVP());
     VERIFY_ARE_EQUAL(true, line_5.Is_KVP(&name, &value));
     VERIFY_ARE_EQUAL("vcidflaerw"s, name);
     VERIFY_ARE_EQUAL("   "s, value);

     return true;
}
