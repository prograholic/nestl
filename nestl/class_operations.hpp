#ifndef NESTL_CLASS_OPERATIONS_HPP
#define NESTL_CLASS_OPERATIONS_HPP

#include <nestl/config.hpp>

#include <nestl/allocator_traits.hpp>

/**
 * @file implementation of class_operations template
 */

namespace nestl
{

template <typename T>
struct class_operations
{
    template <typename Allocator, typename ... Args>
    static void construct(typename Allocator::operation_error& err, T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		nestl::allocator_traits<Allocator>::construct(err, alloc, ptr, std::forward<Args>(args) ...);
    }

    template <typename OperationError, typename Y>
    static void assign(OperationError& /* err */, T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
		dest = std::forward<Y>(src);
    }
};


namespace detail
{

template<typename T, typename Allocator, typename ... Args>
void construct(typename Allocator::operation_error& err, T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	nestl::class_operations<T>::construct(err, ptr, alloc, std::forward<Args>(args) ...);
}

template<typename OperationError, typename T, typename ... Args>
void assign(OperationError& err, T& dest, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	nestl::class_operations<T>::assign(err, dest, std::forward<Args>(args) ...);
}

} // namespace detail
} // namespace nestl

#endif // NESTL_CLASS_OPERATIONS_HPP
