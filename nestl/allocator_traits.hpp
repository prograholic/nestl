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
#include <nestl/exception_support.hpp>

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
        return nestl::numeric_limits<SizeType>::max();
    }
};

template <typename Allocator, typename SizeType>
struct max_size_helper<Allocator, SizeType, true>
{
    static SizeType max_size(const Allocator& alloc) noexcept
    {
        return alloc.max_size();
    }
};


template <typename Allocator, bool>
struct construct_helper
{
    template <typename U, typename ... Args>
    static void construct(Allocator& /* alloc */, U* ptr, Args&& ... args) noexcept
    {
        ::new(static_cast<void*>(ptr)) U(nestl::forward<Args>(args)...);
    }
};

template <typename Allocator>
struct construct_helper<Allocator, true>
{
    template <typename U, typename ... Args>
    static void construct(Allocator& alloc, U* ptr, Args&& ... args) noexcept
    {
        alloc.construct(ptr, nestl::forward<Args>(args)...);
    }
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

    NESTL_SELECT_NESTED_TYPE(Allocator, propagate_on_container_move_assignment, nestl::false_type);
    typedef nestl_nested_type_propagate_on_container_move_assignment propagate_on_container_move_assignment;

    NESTL_SELECT_NESTED_TYPE(Allocator, size_type, nestl::size_t);
    typedef nestl_nested_type_size_type size_type;
    
    typedef can_deal_with_exceptions<value_type> detail_can_deal_with_exceptions_t;
    NESTL_SELECT_NESTED_TYPE(Allocator, has_exceptions, detail_can_deal_with_exceptions_t);
    typedef nestl_nested_type_has_exceptions has_exceptions;
    
    typedef can_deal_with_noexcept<value_type> detail_can_deal_with_noexcept_t;
    NESTL_SELECT_NESTED_TYPE(Allocator, has_noexcept, detail_can_deal_with_noexcept_t);
    typedef nestl_nested_type_has_noexcept has_noexcept;
    
    
    
    static_assert(has_exceptions::value || has_noexcept::value, "objects of given type cannot be handled by this allocator");


    /**
     * @note Each allocator should provide method allocate
     */
    static typename enable_if<has_exceptions, pointer>::type
    allocate(Allocator& alloc, size_type n, void* hint = 0)
    {
        return alloc.allocate(n, hint);
    }
    
    /**
     * @note Each allocator should provide method allocate_nothrow
     */
    static typename enable_if<has_noexcept, pointer>::type
    allocate_nothrow(Allocator& alloc, size_type n, void* hint = 0) noexcept
    {
        return alloc.allocate_nothrow(n, hint);
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

    template<typename U, typename ... Args>
    static void construct(Allocator& alloc, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        typedef has_construct_impl<Allocator, size_type(Allocator::*)(U*, Args...)> has_construct_method;
        detail::construct_helper<Allocator, has_construct_method::value>::construct(alloc, ptr, nestl::forward<Args>(args) ...);
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
