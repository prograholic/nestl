#ifndef NESTL_ALLOCATOR_TRAITS_HPP
#define NESTL_ALLOCATOR_TRAITS_HPP

#include <nestl/config.hpp>
#include <nestl/cstddef.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/move.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/declval.hpp>
#include <nestl/numeric_limits.hpp>
#include <nestl/assert.hpp>
#include <nestl/detail/select_type.hpp>
#include <nestl/forward.hpp>

namespace nestl
{


template <typename Allocator>
struct allocator_traits
{
    typedef Allocator                           allocator_type;
    typedef typename allocator_type::value_type value_type;

    NESTL_SELECT_NESTED_TYPE(Allocator, pointer, value_type*);
    typedef nestl_nested_type_pointer pointer;

    NESTL_SELECT_NESTED_TYPE(Allocator, const_pointer, const value_type*);
    typedef nestl_nested_type_const_pointer const_pointer;

    NESTL_SELECT_NESTED_TYPE(Allocator, propagate_on_container_move_assignment, nestl::false_type);
    typedef nestl_nested_type_propagate_on_container_move_assignment propagate_on_container_move_assignment;

    NESTL_SELECT_NESTED_TYPE(Allocator, size_type, nestl::size_t);
    typedef nestl_nested_type_size_type size_type;


    /**
     * @note Each allocator should provide method allocate
     */
    static pointer allocate(Allocator& alloc, size_type n, void* hint = 0)
    {
        return alloc.allocate(n, hint);
    }

    /**
     * @note Each allocator should provide method deallocate
     */
    template<typename U>
    static void deallocate(Allocator& alloc, U* ptr, size_type n)
    {
        alloc.deallocate(ptr, n);
    }


    NESTL_HAS_METHOD(Allocator, destroy);

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
        destroy_helper(has_destroy_member(), alloc, ptr);
    }


    NESTL_HAS_METHOD(Allocator, max_size);

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
        return max_size_helper(has_max_size_member(), alloc);
    }


    NESTL_HAS_METHOD(Allocator, construct);

#if NESTL_HAS_VARIADIC_TEMPLATES

    template<typename U, typename ... Args>
    static void construct_helper(Allocator& alloc, const nestl::true_type& /* trueVal */, U* ptr, Args&& ... args)
    {
        return alloc.construct(ptr, nestl::forward<Args>(args) ...);
    }

    template<typename U, typename ... Args>
    static void construct_helper(Allocator& /* alloc */, const nestl::false_type& /* falseVal */, U* ptr, Args&& ... args)
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(nestl::forward<Args>(args) ...);
    }

    template<typename U, typename ... Args>
    static void construct(Allocator& alloc, U* ptr, Args&& ... args)
    {
        return construct_helper(alloc, has_construct_member(), ptr, nestl::forward<Args>(args) ...);
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
        construct_helper(alloc, has_construct_member(), ptr, arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */


#undef NESTL_ALLOC_DECLARE_STATIC_METHOD
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
