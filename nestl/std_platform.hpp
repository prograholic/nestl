#ifndef NESTL_STD_PLATFORM_HPP
#define NESTL_STD_PLATFORM_HPP

#include <cstddef>
#include <cassert>
#include <cerrno>
#include <new>

#define NESTL_PLATFORM_ASSERT assert

#define NESTL_ENOMEM          ENOMEM
#define NESTL_EINVAL          EINVAL
#define NESTL_EOVERFLOW       EOVERFLOW

namespace nestl
{

namespace platform
{

using std::size_t;
using std::ptrdiff_t;

} // namespace platform

} // namespace nestl


#endif /* NESTL_STD_PLATFORM_HPP */
