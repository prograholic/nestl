#ifndef NESTL_TYPE_TRAITS_HPP
#define NESTL_TYPE_TRAITS_HPP

#include <nestl/config.hpp>

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

#endif /* NESTL_HAS_RVALUE_REF */


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


template<typename>
struct is_lvalue_reference : public false_type
{
};

template<typename T>
struct is_lvalue_reference<T&> : public true_type
{
};

} // namespace nestl

#endif // NESTL_TYPE_TRAITS_HPP
