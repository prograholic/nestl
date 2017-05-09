#ifndef NESTL_ALLOCATOR_TRAITS_HPP
#define NESTL_ALLOCATOR_TRAITS_HPP

#include <nestl/config.hpp>
#include <nestl/detail/select_type.hpp>

#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/detail/allocator_traits_construct_helper.hpp>
#include <nestl/no_exceptions/detail/allocator_traits_construct_helper.hpp>

#include <type_traits>
#include <limits>
#include <utility>

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


template <typename Allocator, bool test>
using construct_helper = exception_support::dispatch<
    has_exceptions::detail::construct_helper<Allocator, test>,
    no_exceptions::detail::construct_helper<Allocator, test>>;

} // namespace detail

template <typename Allocator>
struct allocator_traits
{
    typedef Allocator                                 allocator_type;
    typedef typename allocator_type::value_type       value_type;

    NESTL_SELECT_NESTED_TYPE(Allocator, pointer, value_type*);
    typedef nestl_nested_type_pointer                 pointer;

    NESTL_SELECT_NESTED_TYPE(Allocator, const_pointer, const value_type*);
    typedef nestl_nested_type_const_pointer           const_pointer;

	NESTL_SELECT_NESTED_TYPE(Allocator, propagate_on_container_move_assignment, std::false_type);
    typedef nestl_nested_type_propagate_on_container_move_assignment propagate_on_container_move_assignment;

	NESTL_SELECT_NESTED_TYPE(Allocator, size_type, std::size_t);
    typedef nestl_nested_type_size_type               size_type;


    /**
     * @note Each allocator should provide method allocate
     */
    template <typename OperationError>
    static pointer allocate(OperationError& err, Allocator& alloc, size_type n, void* hint = 0) NESTL_NOEXCEPT_SPEC
    {
        return alloc.allocate(err, n, hint);
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

    template<typename OperationError, typename U, typename ... Args>
    static void construct(OperationError& err, Allocator& alloc, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        typedef has_construct_impl<Allocator, size_type(Allocator::*)(OperationError&, U*, Args...)> has_construct_method;
		detail::construct_helper<Allocator, has_construct_method::value>::construct(err, alloc, ptr, std::forward<Args>(args) ...);
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
