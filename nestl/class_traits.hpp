#ifndef NESTL_CLASS_TRAITS_HPP
#define NESTL_CLASS_TRAITS_HPP

#include <nestl/config.hpp>

#include <nestl/allocator_traits.hpp>

/**
 * @file implementation of class_traits template
 */

namespace nestl
{

template <typename T>
struct class_traits
{
    template <typename OperationError, typename Allocator, typename ... Args>
    static OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		nestl::allocator_traits<Allocator>::construct(alloc, ptr, std::forward<Args>(args) ...);

        return OperationError();
    }

    template <typename OperationError, typename Y>
    static OperationError assign(T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
		dest = std::forward<Y>(src);

        return OperationError();
    }
};


namespace detail
{

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	return nestl::class_traits<T>:: template construct<OperationError>(ptr, alloc, std::forward<Args>(args) ...);
}

template<typename OperationError, typename T, typename ... Args>
OperationError assign(T& dest, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	return nestl::class_traits<T>:: template assign<OperationError>(dest, std::forward<Args>(args) ...);
}

} // namespace detail
} // namespace nestl

#endif // NESTL_CLASS_TRAITS_HPP
