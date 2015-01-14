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
    /**
     * @note differ from std::numeric_limits::max, because of weird macro max
     */
    static NESTL_CONSTEXPR unsigned int max_value() NESTL_NOEXCEPT_SPEC
    {
        return ~static_cast<unsigned int>(0);
    }
};

}

#endif /* NESTL_NUMERIC_LIMITS_HPP */
