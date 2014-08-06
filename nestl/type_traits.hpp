#ifndef NESTL_TYPE_TRAITS_HPP
#define NESTL_TYPE_TRAITS_HPP

#include <nestl/config.hpp>

#include <type_traits>

namespace nestl
{

namespace detail
{

template <typename T>
class has_assign_copy_member_impl
{
    typedef char has_method;
    typedef long does_not_has_method;

    static_assert(sizeof(has_method) != sizeof(does_not_has_method), "please, select types with different size");

    template <typename C> static has_method test( decltype(&C::assign_copy) ) ;
    template <typename C> static does_not_has_method test(...);

public:
    typedef typename std::conditional
    <
        sizeof(test<T>(0)) == sizeof(has_method),
        std::true_type,
        std::false_type
    >::type type;
};

}


template <typename T>
struct has_assign_copy_member : public detail::has_assign_copy_member_impl<T>::type
{
};

} // namespace nestl

#endif // NESTL_TYPE_TRAITS_HPP
