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
    static constexpr unsigned int max() noexcept
    {
        return ~static_cast<unsigned int>(0);
    }
};


template <>
struct numeric_limits<unsigned long int>
{
    static constexpr unsigned long int max() noexcept
    {
        return ~static_cast<unsigned long int>(0);
    }
};

}

#endif /* NESTL_NUMERIC_LIMITS_HPP */
