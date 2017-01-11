#ifndef NESTL_FORWARD_HPP
#define NESTL_FORWARD_HPP

#include <nestl/config.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/static_assert.hpp>

namespace nestl
{

template<typename T>
constexpr T&&
forward(typename nestl::remove_reference<T>::type& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&&
forward(typename nestl::remove_reference<T>::type&& t) noexcept
{
    NESTL_STATIC_ASSERT(!nestl::is_lvalue_reference<T>::value,
                        "template argument  substituting T is an lvalue reference type");
    return static_cast<T&&>(t);
}


} // namespace nestl


#endif /* NESTL_FORWARD_HPP */
