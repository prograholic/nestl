#ifndef NESTL_ALLOCATOR_TRAITS_HPP
#define NESTL_ALLOCATOR_TRAITS_HPP

#include <nestl/config.hpp>
#include <nestl/cstddef.hpp>
#include <nestl/move.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/declval.hpp>
#include <nestl/numeric_limits.hpp>
#include <nestl/assert.hpp>
#include <nestl/detail/select_type.hpp>
#include <nestl/forward.hpp>

#include <type_traits>
#include <limits>

namespace nestl
{

namespace detail
{

template <typename Allocator, bool>
struct destroy_helper
{
    template <typename U>
    static void destroy(Allocator& /* alloc */, U* ptr) NESTL_NOEXCEPT_SPEC
    {
        ptr->~U();
    }
};

template <typename Allocator>
struct destroy_helper<Allocator, true>
{
    template <typename U>
    static void destroy(Allocator& alloc, U* ptr) NESTL_NOEXCEPT_SPEC
    {
        alloc.destroy(ptr);
    }
};




template <typename Allocator, typename SizeType, bool>
struct max_size_helper
{
    static SizeType max_size(const Allocator& /* alloc */) NESTL_NOEXCEPT_SPEC
    {
        return std::numeric_limits<SizeType>::max();
    }
};

template <typename Allocator, typename SizeType>
struct max_size_helper<Allocator, SizeType, true>
{
    static SizeType max_size(const Allocator& alloc) NESTL_NOEXCEPT_SPEC
    {
        return alloc.max_size();
    }
};


template <typename Allocator, bool>
struct construct_helper
{
#if NESTL_HAS_VARIADIC_TEMPLATES

    template <typename U, typename ... Args>
    static void construct(Allocator& /* alloc */, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        ::new(static_cast<void*>(ptr)) U(nestl::forward<Args>(args)...);
    }

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

    template <typename U>
    static void construct(Allocator& /* alloc */, U* ptr) NESTL_NOEXCEPT_SPEC
    {
        ::new(static_cast<void*>(ptr)) U();
    }

    template <typename U, typename Arg>
    static void construct(Allocator& /* alloc */, U* ptr, const Arg& arg) NESTL_NOEXCEPT_SPEC
    {
        ::new(static_cast<void*>(ptr)) U(arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */
};

template <typename Allocator>
struct construct_helper<Allocator, true>
{
#if NESTL_HAS_VARIADIC_TEMPLATES

    template <typename U, typename ... Args>
    static void construct(Allocator& alloc, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr, nestl::forward<Args>(args)...);
    }

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

    template <typename U>
    static void construct(Allocator& alloc, U* ptr) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);
    }

    template <typename U, typename Arg>
    static void construct(Allocator& alloc, U* ptr, const Arg& arg) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr, arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */
};



} // namespace detail

template <typename Allocator>
struct allocator_traits
{
    typedef Allocator                           allocator_type;
    typedef typename allocator_type::value_type value_type;

    NESTL_SELECT_NESTED_TYPE(Allocator, pointer, value_type*);
    typedef nestl_nested_type_pointer pointer;

    NESTL_SELECT_NESTED_TYPE(Allocator, const_pointer, const value_type*);
    typedef nestl_nested_type_const_pointer const_pointer;

	NESTL_SELECT_NESTED_TYPE(Allocator, propagate_on_container_move_assignment, std::false_type);
    typedef nestl_nested_type_propagate_on_container_move_assignment propagate_on_container_move_assignment;

	NESTL_SELECT_NESTED_TYPE(Allocator, size_type, std::size_t);
    typedef nestl_nested_type_size_type size_type;


    /**
     * @note Each allocator should provide method allocate
     */
    static pointer allocate(Allocator& alloc, size_type n, void* hint = 0) NESTL_NOEXCEPT_SPEC
    {
        return alloc.allocate(n, hint);
    }

    /**
     * @note Each allocator should provide method deallocate
     */
    template<typename U>
    static void deallocate(Allocator& alloc, U* ptr, size_type n) NESTL_NOEXCEPT_SPEC
    {
        alloc.deallocate(ptr, n);
    }

    NESTL_CHECK_METHOD_WITH_SIGNATURE(Allocator, destroy);

    template<typename U>
    static void destroy(Allocator& alloc, U* ptr) NESTL_NOEXCEPT_SPEC
    {
        typedef has_destroy_impl<Allocator, void(Allocator::*)(U*)> has_destroy_method;
        detail::destroy_helper<Allocator, has_destroy_method::value>::destroy(alloc, ptr);
    }


    NESTL_CHECK_METHOD_WITH_SIGNATURE(Allocator, max_size);

    static size_type max_size(const Allocator& alloc) NESTL_NOEXCEPT_SPEC
    {
        typedef has_max_size_impl<Allocator, size_type(Allocator::*)() const> has_max_size_method;
        return detail::max_size_helper<Allocator, size_type, has_max_size_method::value>::max_size(alloc);
    }


    NESTL_CHECK_METHOD_WITH_SIGNATURE(Allocator, construct);

#if NESTL_HAS_VARIADIC_TEMPLATES

    template<typename U, typename ... Args>
    static void construct(Allocator& alloc, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        typedef has_construct_impl<Allocator, size_type(Allocator::*)(U*, Args...)> has_construct_method;
        detail::construct_helper<Allocator, has_construct_method::value>::construct(alloc, ptr, nestl::forward<Args>(args) ...);
    }

#else /* NESTL_HAS_VARIADIC_TEMPLATES */


    template<typename U>
    static void construct(Allocator& alloc, U* ptr) NESTL_NOEXCEPT_SPEC
    {
        typedef has_construct_impl<Allocator, size_type(Allocator::*)(U*)> has_construct_method;
        detail::construct_helper<Allocator, has_construct_method::value>::construct(alloc, ptr);
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
        typedef has_construct_impl<Allocator, size_type(Allocator::*)(U*, const Arg&)> has_construct_method;
        detail::construct_helper<Allocator, has_construct_method::value>::construct(alloc, ptr, arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */


#undef NESTL_ALLOC_DECLARE_STATIC_METHOD
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
