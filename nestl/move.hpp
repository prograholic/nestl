#ifndef NESTL_MOVE_HPP
#define NESTL_MOVE_HPP

#include <nestl/config.hpp>


#if NESTL_ENABLE_CXX11 && NESTL_USE_STD

#include <utility>

namespace nestl
{
using std::move;
}

#else /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */

#if NESTL_HAS_RVALUE_REF

#include <nestl/type_traits.hpp>

namespace nestl
{

template<typename T>
NESTL_CONSTEXPR typename nestl::remove_reference<T>::type&&
move(Tp&& t) NESTL_NOEXCEPT_SPEC
{
    return static_cast<typename nestl::remove_reference<T>::type&&>(t);
}

}

#endif /* NESTL_HAS_RVALUE_REF */

#endif /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */


#endif /* NESTL_MOVE_HPP */
