#include "BigNum.h"

#include <print>

int main(int argc, char* argv[])
{
    std::println("{}", std::string{BigNum{}});
    std::println("{}", std::string{BigNum{uint32_t{5}}});
    std::println("{}", std::string{BigNum{uint32_t{0x1234bad}}});
    std::println("{}", std::string{BigNum{uint64_t{5}}});
    std::println("{}", std::string{BigNum{uint64_t{0x1234556badbeefull}}});

    BigNum n1{0x1234556};
    BigNum n2{0x4444};
    std::println("{} + {} = {}", std::string{n1}, std::string{n2}, std::string{n1 + n2});

    std::println("{} {} {}", n1 < n2, n1 == n2, n1 > n2);
    std::println("{} {} {}", n1 <= n2, n1 == n2, n1 >= n2);

    BigNum n4{0xfedcba9876543211};
    BigNum n5{0x0123456789abcdef};
    std::println("{} + {} = {}", std::string{n4}, std::string{n5}, std::string{n4 + n5});

    std::println("-0 = {}", std::string{-BigNum{0}});
    std::println("-5 = {}", std::string{-BigNum{5}});

    BigNum n6{0x5};
    BigNum n7{0x5};
    std::println("{} x {} = {}", std::string{n6}, std::string{n7}, std::string{n6 * n7});

    BigNum n8{0x100000000};
    BigNum n9{0x100000000};
    std::println("{} x {} = {}", std::string{n8}, std::string{n9}, std::string{n8 * n9});

    BigNum na{0x00000000};
    BigNum nb{0x00000000};
    std::println("{} x {} = {}", std::string{na}, std::string{nb}, std::string{na * nb});
    std::println("{}, {}, {}", na.Is_Zero(), nb.Is_Zero(), (na * nb).Is_Zero());

    return 0;
}
