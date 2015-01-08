#ifndef NESTL_MPL_HPP
#define NESTL_MPL_HPP

/**
 * @file This file provides template metaprogramming functions (like boost::mpl)
 */


#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>

namespace nestl
{

namespace mpl
{

#if defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES)

template <typename ...>
struct and_;

template <>
struct and_<> : public nestl::true_type
{
};

template <typename T>
struct and_<T> : public T
{
};

template <typename T1, typename T2>
struct and_<T1, T2> : public nestl::conditional<T1::value, T2, T1>::type
{
};

template <typename T1, typename T2, typename T3, typename ... Tn>
struct and_<T1, T2, T3, Tn...> : public nestl::conditional<T1::value, and_<T2, T3, Tn...>, T1>::type
{
};

#else /* defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES) */


namespace detail
{

template<bool , typename T1, typename T2, typename T3, typename T4>
struct and_impl : public nestl::false_type
{
};

template<typename T1, typename T2, typename T3, typename T4>
struct and_impl<true, T1, T2, T3, T4> : public and_impl<T1::value, T2, T3, T4, nestl::true_type>
{
};

template<>
struct and_impl<true, nestl::true_type, nestl::true_type, nestl::true_type, nestl::true_type> : public nestl::true_type
{
};

} // namespace detail


template <
    typename T1 = nestl::true_type,
    typename T2 = nestl::true_type,
    typename T3 = nestl::true_type,
    typename T4 = nestl::true_type,
    typename T5 = nestl::true_type>
struct and_ : public detail::and_impl<T1::value, T2, T3, T4, T5>
{
};




#endif /* defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES) */


} // namespace mpl

} // namespace nestl

#endif /* NESTL_MPL_HPP */
