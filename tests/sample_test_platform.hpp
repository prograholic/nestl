#ifndef NESTL_TESTS_SAMPLE_TEST_PLATFORM_HPP
#define NESTL_TESTS_SAMPLE_TEST_PLATFORM_HPP

#if defined(__GNUC__) || defined(__clang__)

inline void gcc_x86_assert(bool x)
{
    if (!x)
    {
        asm("int3");
    }
}

#   define NESTL_PLATFORM_ASSERT(x) gcc_x86_assert(!!(x))

#endif /* __GNUC__ */


#if defined(_MSC_VER)

#   pragma intrinsic(__debugbreak)

inline void msvc_assert(bool x)
{
    if (!x)
    {
        __debugbreak();
    }
}

#   define NESTL_PLATFORM_ASSERT(x) msvc_assert(!!(x))

#endif /* _MSC_VER */


namespace nestl
{

namespace platform
{

typedef unsigned int size_t;
typedef int          ptrdiff_t;

} // namespace platform

} // namespace nestl

#include <new>

/**
 * Following defines are needed for system_error.hpp header
 */
#define NESTL_ENOMEM    12
#define NESTL_EINVAL    22
#define NESTL_EOVERFLOW 75


#endif /* NESTL_TESTS_SAMPLE_TEST_PLATFORM_HPP */
