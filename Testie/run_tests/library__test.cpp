#include "library.h"

#include <testie.h>

BEGIN_TEST_GROUP(Library)
     DECLARE_TEST(Constructor)
     DECLARE_TEST(Get_Function)
     DECLARE_TEST(Call_Function)
END_TEST_GROUP(Library)

DEFINE_TEST(Library, Constructor)
{
     auto library_name = std::string_view{"run_tests__unit_tests.dll"};
     Library test(library_name);

     VERIFY_ARE_EQUAL(library_name, test.Name());
     VERIFY_ARE_NOT_EQUAL((PVOID) NULL, (PVOID) test.Handle());
     VERIFY_ARE_EQUAL(S_OK, test.Last_Error());

     return true;
}

DEFINE_TEST(Library, Get_Function)
{
     auto library_name = std::string_view{"run_tests__unit_tests.dll"};
     Library test(library_name);

     VERIFY_ARE_NOT_EQUAL((PVOID) NULL, test.Get_Function<PVOID>("Get_Test_Group_Count"));
     VERIFY_ARE_NOT_EQUAL((PVOID) NULL, test.Get_Function<PVOID>("Get_Test_Groups"));
     VERIFY_ARE_EQUAL((PVOID) NULL, test.Get_Function<PVOID>("Function_Which_Does_Not_Exist"));

     return true;
}

DEFINE_TEST(Library, Call_Function)
{
     auto library_name = std::string_view{"run_tests__unit_tests.dll"};
     Library test(library_name);

     size_t count{};
     HRESULT result{E_UNEXPECTED};

     result = test.Call_Function("Get_Test_Group_Count", count);
     VERIFY_ARE_EQUAL(S_OK, result);
     VERIFY_ARE_NOT_EQUAL(size_t(0), count);

     Testie::Test_Group** test_group_array{};
     result = test.Call_Function("Get_Test_Groups", test_group_array);
     VERIFY_ARE_EQUAL(S_OK, result);
     VERIFY_ARE_NOT_EQUAL((PVOID) NULL, (PVOID) test_group_array);

     return true;
}
