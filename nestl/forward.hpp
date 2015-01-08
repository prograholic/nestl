#ifndef NESTL_FORWARD_HPP
#define NESTL_FORWARD_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)

#   if defined(NESTL_CONFIG_HAS_STD)

#       include <utility>

namespace nestl
{

using std::forward;

} // namespace nestl

#   else /* defined(NESTL_CONFIG_HAS_STD) */

#       include <nestl/type_traits.hpp>
#       include <nestl/static_assert.hpp>

namespace nestl
{

template<typename T>
NESTL_CONSTEXPR T&&
forward(typename nestl::remove_reference<T>::type& t) NESTL_NOEXCEPT_SPEC
{
    return static_cast<T&&>(t);
}

template<typename T>
NESTL_CONSTEXPR T&&
forward(typename nestl::remove_reference<T>::type&& t) NESTL_NOEXCEPT_SPEC
{
    NESTL_STATIC_ASSERT(!nestl::is_lvalue_reference<T>::value,
                        "template argument substituting T is an lvalue reference type");
    return static_cast<T&&>(t);
}


} // namespace nestl

#   endif  /* defined(NESTL_CONFIG_HAS_STD) */


#else /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */

/// forward is not available

#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */

#endif /* NESTL_FORWARD_HPP */
