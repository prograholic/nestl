#ifndef NESTL_HAS_EXCEPTIONS_ALLOCATOR_TRAITS_HPP
#define NESTL_HAS_EXCEPTIONS_ALLOCATOR_TRAITS_HPP

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
    template <typename U, typename ... Args>
    static void construct(typename Allocator::operation_error& err, Allocator& /* alloc */, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        try
        {
		    ::new(static_cast<void*>(ptr)) U(std::forward<Args>(args)...);
        }
        catch (...)
        {
            err = typename Allocator::operation_error(::std::current_exception());
        }
    }
};

template <typename Allocator>
struct construct_helper<Allocator, true>
{
    template <typename U, typename ... Args>
    static void construct(typename Allocator::operation_error& err, Allocator& alloc, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		alloc.construct(err, ptr, std::forward<Args>(args)...);
    }
};



} // namespace detail
} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_ALLOCATOR_TRAITS_HPP */
