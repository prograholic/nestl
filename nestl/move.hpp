/**
 * @file Provides declaration of nestl::move function
 *
 * @note Implementation is taken from adobe ASL library
 */


#ifndef NESTL_MOVE_HPP
#define NESTL_MOVE_HPP

#include <nestl/config.hpp>

#include <nestl/detail/select_type.hpp>
#include <nestl/type_traits.hpp>

namespace nestl
{

template<typename T>
constexpr typename nestl::remove_reference<T>::type&&
move(T&& t) noexcept
{
    return static_cast<typename nestl::remove_reference<T>::type&&>(t);
}

} // namespace nestl

#endif /* NESTL_MOVE_HPP */
