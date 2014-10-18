#ifndef NESTL_STATIC_ASSERT_HPP
#define NESTL_STATIC_ASSERT_HPP

#include <nestl/config.hpp>


#if NESTL_HAS_STATIC_ASSERT

#   define NESTL_STATIC_ASSERT(x, msg) static_assert(x, msg)

#else /* NESTL_HAS_STATIC_ASSERT */

namespace nestl
{
template <bool>
struct NESTL_ASSERTION_FAILURE;

template <>
struct NESTL_ASSERTION_FAILURE<true> {enum {value = 1}; };

} // namespace nestl


#define NESTL_STATIC_ASSERT(x, msg) \
    typedef unsigned char NESTL_JOIN(nestl_assertion_failure, __LINE__) [::nestl::NESTL_ASSERTION_FAILURE<static_cast<bool>(x)>::value];


#endif /* NESTL_HAS_STATIC_ASSERT */

#endif /* NESTL_STATIC_ASSERT_HPP */
