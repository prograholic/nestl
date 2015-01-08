#ifndef NESTL_SWAP_HPP
#define NESTL_SWAP_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#if defined(NESTL_CONFIG_HAS_CXX11)
#   include <utility>
#else /* defined(NESTL_CONFIG_HAS_CXX11) */
#   include <algorithm>
#endif /* defined(NESTL_CONFIG_HAS_CXX11) */

namespace nestl
{

using std::swap;

} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#include <nestl/move.hpp>

namespace nestl
{

template<typename T>
inline void
swap(T& a, T& b)
{
    T tmp = NESTL_MOVE_IF_SUPPORTED(a);
    a = NESTL_MOVE_IF_SUPPORTED(b);
    b = NESTL_MOVE_IF_SUPPORTED(tmp);
  }
} // namespace nestl

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_SWAP_HPP */
