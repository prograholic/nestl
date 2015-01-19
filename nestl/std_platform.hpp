#ifndef NESTL_STD_PLATFORM_HPP
#define NESTL_STD_PLATFORM_HPP

#include <cstddef>
#include <cassert>
#include <cerrno>
#include <new>

#define NESTL_PLATFORM_ASSERT assert

#define NESTL_ENOMEM          ENOMEM
#define NESTL_EINVAL          EINVAL

#if defined(EOVERFLOW)
#   define NESTL_EOVERFLOW    EOVERFLOW
#else /* defined(EOVERFLOW) */
/**
 * @note msvc-2008 (maybe other too) does not have EOVERFLOW constant
 */
#   define NESTL_EOVERFLOW    75
#endif /* defined(EOVERFLOW) */

namespace nestl
{

namespace platform
{

using std::size_t;
using std::ptrdiff_t;

} // namespace platform

} // namespace nestl


#endif /* NESTL_STD_PLATFORM_HPP */
