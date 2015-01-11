/**
 * @file Provides declaration nf nestl::move function
 *
 * Also provides convenience macro NESTL_MOVE_IF_SUPPORTED
 * this macro transformed to nestl::move if compiler has support of rvalue reference
 */


#ifndef NESTL_MOVE_HPP
#define NESTL_MOVE_HPP

#include <nestl/config.hpp>

#if NESTL_HAS_RVALUE_REF

#include <nestl/type_traits.hpp>

namespace nestl
{

template<typename T>
NESTL_CONSTEXPR typename nestl::remove_reference<T>::type&&
move(T&& t) NESTL_NOEXCEPT_SPEC
{
    return static_cast<typename nestl::remove_reference<T>::type&&>(t);
}

}

#endif /* NESTL_HAS_RVALUE_REF */

#if NESTL_HAS_RVALUE_REF
#   define NESTL_MOVE_IF_SUPPORTED(x) nestl::move(x)
#else /* NESTL_HAS_RVALUE_REF */
#   define NESTL_MOVE_IF_SUPPORTED(x) x
#endif /* NESTL_HAS_RVALUE_REF */


#endif /* NESTL_MOVE_HPP */
