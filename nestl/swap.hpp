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

namespace nestl
{
/// @todo add implementation of swap function
}

#endif /* NESTL_USE_STD */

#endif /* NESTL_SWAP_HPP */
