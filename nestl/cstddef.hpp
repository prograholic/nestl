#ifndef NESTL_CSTDDEF_HPP
#define NESTL_CSTDDEF_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#   include <cstddef>

namespace nestl
{
using std::size_t;
using std::ptrdiff_t;
} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

/**
 * @note nestl::platform::* types should be declared in header NESTL_PLATFORM_HEADER
 */
namespace nestl
{

using nestl::platform::size_t;
using nestl::platform::ptrdiff_t;

} // namespace nestl


#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_CSTDDEF_HPP */
