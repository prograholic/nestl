#ifndef NESTL_PAIR_HPP
#define NESTL_PAIR_HPP

#include <nestl/config.hpp>

namespace nestl
{

template <typename T1, typename T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;

    first_type first;
    second_type second;


    NESTL_CONSTEXPR pair():
        first(),
        second()
    {
    }

    NESTL_CONSTEXPR pair(const T1& a, const T2& b)
        : first(a)
        , second(b)
    {
    }


    template <typename U1, typename U2>
    pair(const pair<U1, U2>& other)
        : first(other.first)
        , second(other.second)
    {
    }

};

} // namespace nestl

#endif /* NESTL_PAIR_HPP */
