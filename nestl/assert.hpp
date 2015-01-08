#ifndef NESTL_ASSERT_HPP
#define NESTL_ASSERT_HPP

#include <nestl/config.hpp>


#if defined(NESTL_CONFIG_HAS_STD_INCLUDES) && !defined(NESTL_CONFIG_HAS_NESTL_CUSTOM_ASSERT)

#   include <cassert>
#   define NESTL_ASSERT(x) assert(x)

#elif defined(NESTL_CONFIG_HAS_NESTL_CUSTOM_ASSERT)

#   define NESTL_ASSERT(x) NESTL_CUSTOM_ASSERT(x)

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) && !defined(NESTL_CONFIG_HAS_NESTL_CUSTOM_ASSERT) */

#   if !defined NESTL_PLATFORM_ASSERT
#       error "Macro NESTL_PLATFORM_ASSERT should be defined in header NESTL_PLATFORM_HEADER"
#   endif /* NESTL_PLATFORM_ASSERT */

#   define NESTL_ASSERT(x) NESTL_PLATFORM_ASSERT(x)

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) && !defined(NESTL_CONFIG_HAS_NESTL_CUSTOM_ASSERT) */


#endif /* NESTL_ASSERT_HPP */
