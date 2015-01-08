#ifndef NESTL_NUMERIC_LIMITS_HPP
#define NESTL_NUMERIC_LIMITS_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#include <limits>

namespace nestl
{
using std::numeric_limits;
} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

namespace nestl
{

template <typename T>
struct numeric_limits;

template <>
struct numeric_limits<unsigned int>
{
    static unsigned int max() NESTL_NOEXCEPT_SPEC
    {
        return ~static_cast<unsigned int>(0);
    }
};

}

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_NUMERIC_LIMITS_HPP */
