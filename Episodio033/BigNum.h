#ifndef BIG_NUM_H__INCLUDED
#define BIG_NUM_H__INCLUDED

#include <cstdint>
#include <format>
#include <list>
#include <stack>
#include <string>

class BigNum
{
    private:
        std::list<uint32_t> _rep;

        uint32_t _sum_with_carry(uint32_t a, uint32_t b, uint64_t& c)
        {
            uint64_t sum = a + b + c;
            c = sum >> 32;
            return static_cast<uint32_t>(sum);
        }

    public:
        BigNum() = default;

        BigNum(uint32_t value):
            _rep{value}
        {}

        BigNum(uint64_t value):
            _rep{static_cast<uint32_t>(value), static_cast<uint32_t>(value >> 32)}
        {}

        BigNum(BigNum const&) = default;
        BigNum(BigNum&&) = default;

        operator std::string() const
        {
            std::string result{};

            if (_rep.empty())
            {
                return result = "0";
            }

            std::stack<uint32_t> stack;

            for (uint32_t d: _rep)
                stack.push(d);

            result.reserve(_rep.size() * 8);

            while (!stack.empty())
            {
                result.append(std::format("{:08x}", stack.top()));
                stack.pop();
            }

            return result;
        }

        BigNum operator+=(BigNum const& that)
        {
            auto this_it = _rep.begin();
            auto that_it = that._rep.begin();
            auto carry = 0ull;

            while ((this_it != _rep.end()) || (that_it != that._rep.end()))
            {
                uint32_t new_digit =
                    _sum_with_carry((this_it == _rep.end())? 0: *this_it,
                                    (that_it == that._rep.end())? 0: *that_it,
                                    carry);

                if (this_it == _rep.end())
                {
                    _rep.push_back(new_digit);
                } else {
                    *this_it = new_digit;
                }

                ++this_it;
                ++that_it;
            }

            while (this_it != _rep.end())
            {
                *this_it = _sum_with_carry(*this_it, 0, carry);
                ++this_it;
            }

            while (that_it != that._rep.end())
            {
                _rep.push_back(_sum_with_carry(*that_it, 0, carry));
                ++that_it;
            }

            if (carry) _rep.push_back(static_cast<uint32_t>(carry));

            return *this;
        }
};

#endif  // BIG_NUM_H__INCLUDED
