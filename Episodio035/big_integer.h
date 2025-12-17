#ifndef BIG_INTEGER_H__INCLUDED
#define BIG_INTEGER_H__INCLUDED

#include <cstdint>
#include <format>
#include <list>
#include <stack>
#include <string>

class Big_Integer
{
    private:
        std::list<uint32_t> _rep;

        uint32_t _sum_with_carry(uint64_t a, uint64_t b, uint64_t& c)
        {
            uint64_t sum = a + b + c;
            c = sum >> 32;

            return static_cast<uint32_t>(sum);
        }

        uint32_t back() const
        {
            return _rep.empty()? 0: _rep.back();
        }

    public:
        Big_Integer() = default;

        explicit Big_Integer(uint64_t value):
            _rep{static_cast<uint32_t>(value), static_cast<uint32_t>(value >> 32)}
        {}

        Big_Integer(Big_Integer const&) = default;
        Big_Integer(Big_Integer&&) = default;

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

        bool Is_Zero() const
        {
            if (_rep.empty())
                return true;

            for (auto const d: _rep)
                if (d != 0)
                    return false;

            return true;
        }

        bool Is_Positive() const
        {
            if (Is_Zero())
                return false;

            return !(back() & 0x80000000);
        }

        bool Is_Negative() const
        {
            return (back() & 0x80000000);
        }

        int Spaceship_With_Zero() const
        {
            if (Is_Zero())
                return 0;

            if (Is_Positive())
                return 1;

            // Is_Negative.
            return -1;
        }

        int operator<=>(Big_Integer const& that) const
        {
            return (that.Is_Zero())?
                Spaceship_With_Zero():
                (*this - that).Spaceship_With_Zero();
        }

        int operator<=>(uint64_t that) const
        {
            return operator<=>(Big_Integer{that});
        }

        bool operator==(Big_Integer const& that) const
        {
            return operator<=>(that) == 0;
        }

        bool operator==(uint64_t that) const
        {
            return operator<=>(that) == 0;
        }

        Big_Integer operator-() const
        {
            Big_Integer result;

            if (Is_Zero())
                return result;

            for (auto const n: _rep)
                result._rep.push_back(~n);

            return result+= Big_Integer{1};
        }

        Big_Integer operator+=(Big_Integer const& that)
        {
            bool this_was_negative = this->Is_Negative();

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

                if (this_it != _rep.end())
                    ++this_it;

                if (that_it != that._rep.end())
                    ++that_it;
            }

            while (this_it != _rep.end())
            {
                *this_it = _sum_with_carry(*this_it, 0, carry);
                ++this_it;
            }

            while (that_it != that._rep.end())
            {
                _rep.push_back(_sum_with_carry(0, *that_it, carry));
                ++that_it;
            }

            if (this_was_negative != that.Is_Negative())
            {
                carry = 0;
            } else
            if (this_was_negative && that.Is_Negative())
            {
               if ((carry == std::numeric_limits<uint32_t>::max()) &&
                   (back() & 0x80000000UL))
               {
                    carry = 0;
               }
            }

            if (carry)
               _rep.push_back(static_cast<uint32_t>(carry));

            return *this;
        }

        friend Big_Integer operator*(Big_Integer const& a, Big_Integer const& b);
        friend Big_Integer operator-(Big_Integer const& a, Big_Integer const& b);
};

inline Big_Integer operator+(Big_Integer const& a, Big_Integer const& b)
{
     return Big_Integer{a}+= b;
}

inline Big_Integer operator-(Big_Integer const& a, Big_Integer const& b)
{
     return Big_Integer{a}+= -b;
}

Big_Integer operator*(Big_Integer const& a, Big_Integer const& b)
{
    Big_Integer result;

    int number_of_zeroes = 0;

    for (auto const d1: b._rep)
    {
        Big_Integer row;

        for (int i = 0; i < number_of_zeroes; ++i)
            row._rep.push_back(0);

        uint64_t carry{};

        for (auto const d2: a._rep)
        {
            auto const product = uint64_t{d1} * uint64_t{d2} + carry;
            carry = product >> 32;
            row._rep.push_back(static_cast<uint32_t>(product));
        }

        result+= row;
        ++number_of_zeroes;
    }

    return result;
}

#endif  // BIG_INTEGER_H__INCLUDED
