#ifndef NESTL_HAS_EXCEPTIONS_DETAIL_ALLOCATOR_TRAITS_CONSTRUCT_HELPER_HPP
#define NESTL_HAS_EXCEPTIONS_DETAIL_ALLOCATOR_TRAITS_CONSTRUCT_HELPER_HPP

#include <nestl/config.hpp>

#include <new>
#include <utility>
#include <exception>

namespace nestl
{
namespace has_exceptions
{
namespace detail
{

template <typename Allocator, bool>
struct construct_helper
{
    template <typename OperationError, typename U, typename ... Args>
    static void construct(OperationError& err, Allocator& /* alloc */, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        try
        {
		    ::new(static_cast<void*>(ptr)) U(std::forward<Args>(args)...);
        }
        catch (...)
        {
            from_exception_ptr(err, std::current_exception());
        }
    }
};

template <typename Allocator>
struct construct_helper<Allocator, true>
{
    template <typename OperationError, typename U, typename ... Args>
    static void construct(OperationError& err, Allocator& alloc, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		alloc.construct(err, ptr, std::forward<Args>(args)...);
    }
};



} // namespace detail
} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_DETAIL_ALLOCATOR_TRAITS_CONSTRUCT_HELPER_HPP */
