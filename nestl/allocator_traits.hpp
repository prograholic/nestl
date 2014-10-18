#ifndef NESTL_ALLOCATOR_TRAITS_HPP
#define NESTL_ALLOCATOR_TRAITS_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD && NESTL_ENABLE_CXX11

#include <memory>

namespace nestl
{

using std::allocator_traits;

} // namespace nestl



#else /* NESTL_USE_STD && NESTL_ENABLE_CXX11 */

#include <nestl/declval.hpp>

namespace nestl
{


template <typename Allocator>
struct allocator_traits
{
    typedef Allocator                           allocator_type;
    typedef typename allocator_type::value_type value_type;


/**
 * @note This macro allows to use nested type of allocator (if it has corresponding type)
 * Otherwise it declares type usign default type
 *
 * @note This macro is taken from stdlibc++
 */
#define NESTL_ALLOC_DECLARE_NESTED_TYPE(NestedType, DefaultType) \
    private: \
        template<typename T> \
        static typename T::NestedType NESTL_##NestedType##_helper(T*); \
        static DefaultType NESTL_##NestedType##_helper(...); \
        typedef NESTL_DECLTYPE(NESTL_##NestedType##_helper((Allocator*)0)) nestl_nested_type_##NestedType; \
    public:

    NESTL_ALLOC_DECLARE_NESTED_TYPE(pointer, value_type*);
    typedef nestl_nested_type_pointer pointer;

    NESTL_ALLOC_DECLARE_NESTED_TYPE(const_pointer, const value_type*);
    typedef nestl_nested_type_const_pointer const_pointer;

    NESTL_ALLOC_DECLARE_NESTED_TYPE(propagate_on_container_move_assignment, nestl::false_type);
    typedef nestl_nested_type_propagate_on_container_move_assignment propagate_on_container_move_assignment;


#undef NESTL_ALLOC_DECLARE_NESTED_TYPE



    template <typename T>
    class has_destroy_member_impl
    {
        typedef char has_method;
        typedef long does_not_has_method;

        NESTL_STATIC_ASSERT(sizeof(has_method) != sizeof(does_not_has_method),
                            "please, select types with different size");

        template <typename C> static has_method test( NESTL_DECLTYPE(&C::destroy) ) ;
        template <typename C> static does_not_has_method test(...);

    public:
        typedef typename nestl::conditional
        <
            sizeof(test<T>(0)) == sizeof(has_method),
            nestl::true_type,
            nestl::false_type
        >::type type;
    };
};

} // namespace nestl

#endif /* NESTL_USE_STD && NESTL_ENABLE_CXX11 */




#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
