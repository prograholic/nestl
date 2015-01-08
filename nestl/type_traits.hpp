#ifndef NESTL_TYPE_TRAITS_HPP
#define NESTL_TYPE_TRAITS_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#include <type_traits>

namespace nestl
{
using std::conditional;
using std::true_type;
using std::false_type;
using std::add_rvalue_reference;
using std::remove_reference;
using std::is_lvalue_reference;
using std::is_class;
using std::is_union;
}

#else /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

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

#if defined(NESTL_CONFIG_HAS_CONSTEXPR)

    static constexpr value_type value = val;
    constexpr operator value_type() const { return value; }
    constexpr value_type operator()() const { return value; }

#else /* defined(NESTL_CONFIG_HAS_CONSTEXPR) */

    enum
    {
        value = val
    };

    operator value_type() const { return static_cast<value_type>(value); }
    value_type operator()() const { return static_cast<value_type>(value); }

#endif /* defined(NESTL_CONFIG_HAS_CONSTEXPR) */
};

#if defined(NESTL_CONFIG_HAS_CONSTEXPR)
    template<typename Type, Type val>
    NESTL_CONSTEXPR Type integral_constant<Type, val>::value;
#endif /* defined(NESTL_CONFIG_HAS_CONSTEXPR) */


/// The type used as a compile-time boolean with true value.
typedef integral_constant<bool, true>     true_type;

/// The type used as a compile-time boolean with false value.
typedef integral_constant<bool, false>    false_type;

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)

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

#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */


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

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)

template<typename T>
struct remove_reference<T&&>
{
    typedef T type;
};

#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */


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


namespace detail
{

template<typename From, typename To>
struct is_convertible_simple
{
private:
    typedef char one;
    struct two
    {
        one x[2];
    };

    static one test(To);
    static two test(...);
    static From makeFrom();

public:
    enum
    {
        value = (sizeof(test(makeFrom())) == sizeof(one))
    };
};

template<typename From, typename To,
    bool = (nestl::is_void<From>::value || nestl::is_void<To>::value || nestl::is_function<To>::value || nestl::is_array<To>::value
    // This special case is here only to avoid warnings.
    || (nestl::is_floating_point<typename nestl::remove_reference<From>::type>::value && nestl::detail::is_int_or_cref<To>::value))>
struct is_convertible_helper
{
    enum
    {
        // "An imaginary lvalue of type From...".
        value = (nestl::detail::is_convertible_simple<typename nestl::add_reference<From>::type, To>::value)
    };
};

template<typename From, typename To>
struct is_convertible_helper<From, To, true>
{
    enum
    {
        value = (nestl::is_void<To>::value || (nestl::detail::is_int_or_cref<To>::value && !nestl::is_void<From>::value))
    };
};

} // namespace detail

template <typename From, typename To>
struct is_convertible : public nestl::integral_constant<bool, detail::is_convertible_helper<From, To>::value>
{
};


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

#endif /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */


#endif // NESTL_TYPE_TRAITS_HPP
