#ifndef NESTL_NO_EXCEPTIONS_ALLOCATOR_TRAITS_HPP
#define NESTL_NO_EXCEPTIONS_ALLOCATOR_TRAITS_HPP

#include <nestl/config.hpp>

#include <new>
#include <utility>

namespace nestl
{
namespace no_exceptions
{
namespace detail
{

template <typename Allocator, bool>
struct construct_helper
{
    template <typename U, typename ... Args>
    static void construct(typename Allocator::operation_error& /* err */, Allocator& /* alloc */, U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		::new(static_cast<void*>(ptr)) U(std::forward<Args>(args)...);
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
} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_ALLOCATOR_TRAITS_HPP */
