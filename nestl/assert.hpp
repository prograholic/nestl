#ifndef NESTL_ASSERT_HPP
#define NESTL_ASSERT_HPP

#include <nestl/config.hpp>


#if NESTL_USE_STD && !defined(NESTL_CUSTOM_ASSERT)
#   include <cassert>
#   define NESTL_ASSERT(x) assert(x)
#else /* NESTL_USE_STD && !defined(NESTL_CUSTOM_ASSERT) */
#   define NESTL_ASSERT(x) NESTL_CUSTOM_ASSERT(x)
#endif /* NESTL_USE_STD && !defined(NESTL_CUSTOM_ASSERT) */





#endif /* NESTL_ASSERT_HPP */
