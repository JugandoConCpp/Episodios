#include "BigNum.h"

#include <print>

int main(int argc, char* argv[])
{
    std::println("{}", std::string{BigNum{}});
    std::println("{}", std::string{BigNum{uint32_t{5}}});
    std::println("{}", std::string{BigNum{uint32_t{0x1234bad}}});
    std::println("{}", std::string{BigNum{uint64_t{5}}});
    std::println("{}", std::string{BigNum{uint64_t{0x1234556badbeefull}}});

    BigNum n1{uint32_t{0x1234556}};
    BigNum n2{uint32_t{0x4444}};
    BigNum n3 = n1;
    n3+= n2;
    std::println("{} + {} = {}", std::string{n1}, std::string{n2}, std::string{n3});

    BigNum n4{uint64_t{0xfedcba9876543211}};
    BigNum n5{uint64_t{0x0123456789abcdef}};
    BigNum n6 = n4;
    n6+= n5;
    std::println("{} + {} = {}", std::string{n4}, std::string{n5}, std::string{n6});

    return 0;
}
