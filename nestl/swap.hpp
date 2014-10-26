#ifndef NESTL_SWAP_HPP
#define NESTL_SWAP_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD

#if NESTL_ENABLE_CXX11
#   include <utility>
#else /* NESTL_ENABLE_CXX11 */
#   include <algorithm>
#endif /* NESTL_ENABLE_CXX11 */

namespace nestl
{

using std::swap;

} // namespace nestl

#else /* NESTL_USE_STD */

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

#endif /* NESTL_USE_STD */

#endif /* NESTL_SWAP_HPP */
