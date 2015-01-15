#ifndef NESTL_NUMERIC_LIMITS_HPP
#define NESTL_NUMERIC_LIMITS_HPP

#include <nestl/config.hpp>

namespace nestl
{

template <typename T>
struct numeric_limits;

template <>
struct numeric_limits<unsigned int>
{
    typedef unsigned int value_type;
    /**
     * @note differ from std::numeric_limits::max, because of weird macro max
     */
    static NESTL_CONSTEXPR value_type max_value() NESTL_NOEXCEPT_SPEC
    {
        return ~static_cast<value_type>(0);
    }
};



template <>
struct numeric_limits<unsigned short>
{
    typedef unsigned short value_type;
    /**
     * @note differ from std::numeric_limits::max, because of weird macro max
     */
    static NESTL_CONSTEXPR value_type max_value() NESTL_NOEXCEPT_SPEC
    {
        return ~static_cast<value_type>(0);
    }
};

}

#endif /* NESTL_NUMERIC_LIMITS_HPP */
