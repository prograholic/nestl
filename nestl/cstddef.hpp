#ifndef NESTL_CSTDDEF_HPP
#define NESTL_CSTDDEF_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD

#   include <cstddef>

namespace nestl
{
using std::size_t;
using std::ptrdiff_t;
} // namespace nestl

#else /* NESTL_USE_STD */

/**
 * @note nestl::platform::* types should be declared in header NESTL_PLATFORM_HEADER
 */
namespace nestl
{

using nestl::platform::size_t;
using nestl::platform::ptrdiff_t;

} // namespace nestl


#endif /* NESTL_USE_STD */

#endif /* NESTL_CSTDDEF_HPP */
