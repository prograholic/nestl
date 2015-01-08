#ifndef NESTL_CONFIG_HPP
#define NESTL_CONFIG_HPP

/**
 * @file config file for nestl project
 */

#define NESTL_COMPILER_CLANG                            1
#define NESTL_COMPILER_GCC                              2
#define NESTL_COMPILER_MSVC                             3

#define NESTL_JOIN(x, y)                                x ## y

#if defined(__clang__)
#   include <nestl/detail/clang.hpp>
#elif defined(__GNUC__)
#   include <nestl/detail/gcc.hpp>
#elif defined(_MSC_VER)
#   include <nestl/detail/msvc.hpp>
#endif

#if !defined(NESTL_COMPILER)
#   error Unknown compiler, please provide corresponding header
#endif /* !defined(NESTL_COMPILER) */


#if !defined(NESTL_NO_STD)

#   define NESTL_CONFIG_HAS_STD_INCLUDES                1

#else /* !defined(NESTL_NO_STD) */

#   if !defined NESTL_PLATFORM_HEADER
#       error "NESTL is not usable without platform-specific header, see nestl/platform.hpp.sample for details"
#   endif /* NESTL_PLATFORM_HEADER */

#   include NESTL_PLATFORM_HEADER

#endif /* NESTL_NO_STD */

#if defined(NESTL_CONFIG_HAS_INITIALIZER_LIST_HEADER) && defined(NESTL_CONFIG_HAS_STD_INCLUDES)
#   define NESTL_CONFIG_HAS_INITIALIZER_LIST            1
#endif /* defined(NESTL_CONFIG_HAS_INITIALIZER_LIST_HEADER) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */






#endif /* NESTL_CONFIG_HPP */
