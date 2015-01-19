#ifndef NESTL_TYPE_TRAITS_HPP
#define NESTL_TYPE_TRAITS_HPP

#include <nestl/config.hpp>

#include <nestl/cstddef.hpp>

namespace nestl
{

template <bool B, class T = void>
struct enable_if_c
{
    typedef T type;
};

template <class T>
struct enable_if_c<false, T>
{
};

template <class Cond, class T = void>
struct enable_if : public enable_if_c<Cond::value, T>
{
};

template <bool B, class T = void>
struct disable_if_c
{
    typedef T type;
};

template <class T>
struct disable_if_c<true, T>
{
};

template <class Cond, class T = void>
struct disable_if : public disable_if_c<Cond::value, T>
{
};


template <bool, typename TrueType, typename FalseType>
struct conditional
{
    typedef FalseType type;
};

template <typename TrueType, typename FalseType>
struct conditional<true, TrueType, FalseType>
{
    typedef TrueType type;
};

template <typename Type, Type val>
struct integral_constant
{
    typedef integral_constant<Type, val> type;
    typedef Type                         value_type;

#if NESTL_HAS_CONSTEXPR

    static constexpr value_type value = val;
    constexpr operator value_type() const { return value; }
    constexpr value_type operator()() const { return value; }

#else /* NESTL_HAS_CONSTEXPR */

    enum
    {
        value = val
    };

    operator value_type() const { return static_cast<value_type>(value); }
    value_type operator()() const { return static_cast<value_type>(value); }

#endif /* NESTL_HAS_CONSTEXPR */
};

#if NESTL_HAS_CONSTEXPR
    template<typename Type, Type val>
    NESTL_CONSTEXPR Type integral_constant<Type, val>::value;
#endif /* NESTL_HAS_CONSTEXPR */


/// The type used as a compile-time boolean with true value.
typedef integral_constant<bool, true>     true_type;

/// The type used as a compile-time boolean with false value.
typedef integral_constant<bool, false>    false_type;




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



template<typename...>
struct or_;

template<>
struct or_<> : public false_type
{
};

template<typename B1>
struct or_<_B1> : public B1
{
};

template<typename B1, typename B2>
struct or_<B1, B2> : public conditional<B1::value, B1, B2>::type
{
};

template<typename B1, typename B2, typename B3, typename... Bn>
struct or_<B1, B2, B3, Bn...> : public conditional<B1::value, B1, or_<B2, B3, Bn...> >::type
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


template<bool , typename T1, typename T2, typename T3, typename T4>
struct or_impl : public nestl::true_type
{
};

template<typename T1, typename T2, typename T3, typename T4>
struct or_impl<false, T1, T2, T3, T4> : public or_impl<T1::value, T2, T3, T4, nestl::false_type>
{
};

template<>
struct or_impl<false, nestl::false_type, nestl::false_type, nestl::false_type, nestl::false_type> : public nestl::false_type
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


template <
    typename T1 = nestl::false_type,
    typename T2 = nestl::false_type,
    typename T3 = nestl::false_type,
    typename T4 = nestl::false_type,
    typename T5 = nestl::false_type>
struct or_ : public detail::or_impl<T1::value, T2, T3, T4, T5>
{
};


template <typename T>
struct not_ : public integral_constant<bool, !T::value>
{
};


#endif /* defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES) */


} // namespace mpl


#if NESTL_HAS_RVALUE_REF

template <typename T>
struct add_rvalue_reference
{
    typedef T&& type;
};

template <typename T>
struct add_rvalue_reference<T&>
{
    typedef T& type;
};

template <typename T>
struct add_rvalue_reference<T&&>
{
    typedef T&& type;
};

#else /* NESTL_HAS_RVALUE_REF */

template <typename T>
struct add_rvalue_reference
{
    typedef T type;
};

template <typename T>
struct add_rvalue_reference<T&>
{
    typedef T& type;
};

#endif /* NESTL_HAS_RVALUE_REF */


template <typename T>
nestl::add_rvalue_reference<T> declval();


template<typename T>
struct remove_reference
{
    typedef T type;
};

template<typename T>
struct remove_reference<T&>
{
    typedef T type;
};

#if NESTL_HAS_RVALUE_REF

template<typename T>
struct remove_reference<T&&>
{
    typedef T type;
};

#endif /* NESTL_HAS_RVALUE_REF */


template<typename T>
struct remove_const
{
    typedef T type;
};

template<typename T>
struct remove_const<const T>
{
    typedef T type;
};

template<typename T>
struct remove_volatile
{
    typedef T type;
};

template<typename T>
struct remove_volatile<volatile T>
{
    typedef T type;
};

template<typename T>
struct remove_cv
{
    typedef typename nestl::remove_volatile<typename nestl::remove_const<T>::type>::type type;
};


template<typename>
struct is_lvalue_reference : public false_type
{
};

template<typename T>
struct is_lvalue_reference<T&> : public true_type
{
};

template<typename>
struct is_rvalue_reference : public false_type
{
};

#if NESTL_HAS_RVALUE_REF

template<typename T>
struct is_rvalue_reference<T&&> : public true_type
{
};

#endif /* NESTL_HAS_RVALUE_REF */


template<typename T, typename U>
struct is_same : public nestl::false_type
{
};

template<typename T>
struct is_same<T, T> : public nestl::true_type
{
};


template<typename T>
struct is_void : public nestl::integral_constant< bool, nestl::is_same<void, typename nestl::remove_cv<T>::type>::value>
{
};


template<typename T>
struct is_reference : public nestl::mpl::or_<is_lvalue_reference<T>, is_rvalue_reference<T> >::type
{
};

#include <nestl/detail/type_traits/is_function.inc>


template<typename>
struct is_array : public false_type
{
};

template<typename T, nestl::size_t size>
struct is_array<T[size]> : public true_type
{
};

template<typename T>
struct is_array<T[]> : public true_type
{
};

#include <nestl/detail/type_traits/is_integral.inc>

namespace detail
{

template<typename>
struct is_floating_point_helper : public false_type
{
};

template<>
struct is_floating_point_helper<float> : public true_type
{
};

template<>
struct is_floating_point_helper<double> : public true_type
{
};

template<>
struct is_floating_point_helper<long double> : public true_type
{
};

} // namespace detail

template<typename T>
struct is_floating_point : public detail::is_floating_point_helper<typename remove_cv<T>::type>::type
{
};

template<typename>
struct is_const : public false_type
{
};

template<typename T>
struct is_const<T const> : public true_type
{
};

template<typename>
struct is_volatile : public false_type
{
};

template<typename T>
struct is_volatile<T volatile> : public true_type
{
};

namespace detail
{

template<typename T, bool = (is_void<T>::value || is_reference<T>::value)>
struct add_reference_helper
{
    typedef T& type;
};

template<typename T>
struct add_reference_helper<T, true>
{
    typedef T  type;
};

} // namespace detail

template<typename T>
struct add_reference : public detail::add_reference_helper<T>
{
};

namespace detail
{
template<typename T>
struct is_int_or_cref
{
    typedef typename remove_reference<T>::type type;
    enum
    {
        value = (is_integral<T>::value
                || (is_integral<type>::value
                && is_const<type>::value
                && !is_volatile<type>::value))
    };
};

} // namespace detail


#include <nestl/detail/type_traits/is_convertible.inc>

/**
 * Both msvc, clang and gcc provide so called "type traits intrinsincs" so we can use them
 */
#define NESTL_TYPE_TRAITS_INTRINSIC_IS_UNION(T) __is_union(T)
#define NESTL_TYPE_TRAITS_INTRINSIC_IS_CLASS(T) __is_class(T)

template <typename T>
struct is_union : public nestl::integral_constant<bool, NESTL_TYPE_TRAITS_INTRINSIC_IS_UNION(T) >
{
};

template <class T>
struct is_class : public nestl::integral_constant<bool, NESTL_TYPE_TRAITS_INTRINSIC_IS_CLASS(T) && !nestl::is_union<T>::value >
{
};

} // namespace nestl

#endif // NESTL_TYPE_TRAITS_HPP
