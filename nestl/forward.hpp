#ifndef NESTL_FORWARD_HPP
#define NESTL_FORWARD_HPP

#include <nestl/config.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/static_assert.hpp>

#if NESTL_HAS_RVALUE_REF

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
                        "template argument  substituting T is an lvalue reference type");
    return static_cast<T&&>(t);
}


} // namespace nestl

#else /* NESTL_HAS_RVALUE_REF */

/// forward is not available

#endif /* NESTL_HAS_RVALUE_REF */

#endif /* NESTL_FORWARD_HPP */
