#ifndef NESTL_NUMERIC_LIMITS_HPP
#define NESTL_NUMERIC_LIMITS_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD

#include <limits>

namespace nestl
{
using std::numeric_limits;
} // namespace nestl

#else /* NESTL_USE_STD */

namespace nestl
{

template <typename T
struct numeric_limits;

}

#endif /* NESTL_USE_STD */

#endif /* NESTL_NUMERIC_LIMITS_HPP */
