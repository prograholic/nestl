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
#include <nestl/numeric_limits.hpp>
#include <nestl/assert.hpp>

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

    NESTL_ALLOC_DECLARE_NESTED_TYPE(size_type, size_t);
    typedef nestl_nested_type_size_type size_type;


#undef NESTL_ALLOC_DECLARE_NESTED_TYPE


/**
 * @note This macro allows to check whether Allocator has necessary member or not
 */
#define NESTL_ALLOC_DECLARE_STATIC_METHOD(methodName) \
    template <typename T> \
    class has_ ## methodName ## _member_impl \
    { \
        typedef char has_method; \
        typedef long does_not_has_method; \
        NESTL_STATIC_ASSERT(sizeof(has_method) != sizeof(does_not_has_method), \
                            "please, select types with different size"); \
        template <typename C> static has_method test( NESTL_DECLTYPE(nestl::declval(&C::methodName)) ) ; \
        template <typename C> static does_not_has_method test(...); \
    public: \
        typedef typename nestl::conditional \
        < \
            sizeof(test<T>(0)) == sizeof(has_method), \
            nestl::true_type, \
            nestl::false_type \
        >::type type; \
    } \

    NESTL_ALLOC_DECLARE_STATIC_METHOD(destroy);

    template<typename U>
    static void destroy_helper(const nestl::true_type& /* trueVal */, Allocator& alloc, U* ptr)
    {
        alloc.destroy(ptr);
    }

    template<typename U>
    static void destroy_helper(const nestl::false_type& /* falseVal */, Allocator& /* alloc */, U* ptr)
    {
        ptr->~U();
    }

    template<typename U>
    static void destroy(Allocator& alloc, U* ptr)
    {
        typedef typename has_destroy_member_impl<Allocator>::type has_destroy_member;
        destroy_helper(has_destroy_member(), alloc, ptr);
    }


    NESTL_ALLOC_DECLARE_STATIC_METHOD(max_size);

    static size_type max_size_helper(const nestl::true_type& /* trueVal */, const Allocator& alloc)
    {
        return alloc.max_size();
    }

    static size_type max_size_helper(const nestl::false_type& /* falseVal */, const Allocator& /* alloc */)
    {
        return nestl::numeric_limits<size_type>::max();
    }

    static size_type max_size(const Allocator& alloc)
    {
        typedef typename has_max_size_member_impl<Allocator>::type has_max_size_member;
        return max_size_helper(has_max_size_member(), alloc);
    }


    NESTL_ALLOC_DECLARE_STATIC_METHOD(construct);

#if NESTL_HAS_VARIADIC_TEMPLATES

    template<typename U, typename ... Args>
    static void construct_helper(Allocator& alloc, const nestl::true_type& /* trueVal */, U* ptr)
    {
        return alloc.construct(ptr, std::forward<Args>(args) ...);
    }

    template<typename U, typename ... Args>
    static void construct_helper(Allocator& /* alloc */, const nestl::false_type& /* falseVal */, U* ptr)
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(std::forward<Args>(args) ...);
    }

    template<typename U, typename ... Args>
    static size_type construct(Allocator& alloc, U* ptr)
    {
        typedef typename has_construct_member_impl<Allocator>::type has_construct_member;
        return construct_helper(alloc, has_construct_member(), ptr, std::forward<Args>(args) ...);
    }

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

    template<typename U>
    static void construct_helper(Allocator& alloc, const nestl::true_type& /* trueVal */, U* ptr)
    {
        return alloc.construct(ptr);
    }

    template<typename U>
    static void construct_helper(Allocator& /* alloc */, const nestl::false_type& /* falseVal */, U* ptr)
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U();
    }

    template<typename U>
    static void construct(Allocator& alloc, U* ptr)
    {
        typedef typename has_construct_member_impl<Allocator>::type has_construct_member;
        construct_helper(alloc, has_construct_member(), ptr);
    }

    template<typename U, typename Arg>
    static void construct_helper(Allocator& alloc, const nestl::true_type& /* trueVal */, U* ptr, const Arg& arg)
    {
        return alloc.construct(ptr, arg);
    }

    template<typename U, typename Arg>
    static void construct_helper(Allocator& /* alloc */, const nestl::false_type& /* falseVal */, U* ptr, const Arg& arg)
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(arg);
    }

    template<typename U, typename Arg>
    static void construct(Allocator& alloc, U* ptr, const Arg& arg)
    {
        typedef typename has_construct_member_impl<Allocator>::type has_construct_member;
        construct_helper(alloc, has_construct_member(), ptr, arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */


#undef NESTL_ALLOC_DECLARE_STATIC_METHOD
};

} // namespace nestl

#endif /* NESTL_USE_STD && NESTL_ENABLE_CXX11 */




#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
