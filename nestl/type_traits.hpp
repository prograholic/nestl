#ifndef NESTL_TYPE_TRAITS_HPP
#define NESTL_TYPE_TRAITS_HPP

#include <nestl/config.hpp>

#if NESTL_ENABLE_CXX11 && NESTL_USE_STD


namespace nestl
{
using std::conditional;
using std::true_type;
using std::false_type;
using std::add_rvalue_reference;
}

#else /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */

namespace nestl
{


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

    static constexpr value_type    value;
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

template <>
struct add_rvalue_reference<T&>
{
    typedef T& type;
};

template <>
struct add_rvalue_reference<T&&>
{
    typedef T&& type;
};

#endif /* NESTL_HAS_RVALUE_REF */

} // namespace nestl

#endif /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */


namespace nestl
{

namespace detail
{

template <typename T>
class has_assign_copy_member_impl
{
    typedef char has_method;
    typedef long does_not_has_method;

    NESTL_STATIC_ASSERT(sizeof(has_method) != sizeof(does_not_has_method),
                        "please, select types with different size");

    template <typename C> static has_method test( NESTL_DECLTYPE(&C::assign_copy) ) ;
    template <typename C> static does_not_has_method test(...);

public:
    typedef typename nestl::conditional
    <
        sizeof(test<T>(0)) == sizeof(has_method),
        nestl::true_type,
        nestl::false_type
    >::type type;
};

}


template <typename T>
struct has_assign_copy_member : public detail::has_assign_copy_member_impl<T>::type
{
};

} // namespace nestl


#endif // NESTL_TYPE_TRAITS_HPP
