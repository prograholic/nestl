#ifndef NESTL_CLASS_TRAITS_HPP
#define NESTL_CLASS_TRAITS_HPP

#include <nestl/config.hpp>

#include <nestl/detail/construct.hpp>

/**
 * @file implementation of class_traits template
 */

namespace nestl
{

template <typename T>
struct class_traits
{
    template <typename OperationError, typename Allocator, typename ... Args>
    static OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) noexcept
    {
        return nestl::detail::construct_impl<OperationError>(ptr, alloc, std::forward<Args>(args) ...);
    }

    template <typename OperationError, typename Y>
    static OperationError assign(T& dest, Y&& src) noexcept
    {
        dest = std::forward<Y>(src);

        return OperationError();
    }
};


namespace detail
{

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) noexcept
{
#define NESTL_CALL_CLASS_TRAITS_CONSTRUCT nestl::class_traits<T>:: template construct<OperationError>(ptr, alloc, std::forward<Args>(args) ...)

    constexpr bool is_nothrow_op = noexcept(NESTL_CALL_CLASS_TRAITS_CONSTRUCT);
    static_assert(is_nothrow_op, "User should provide nothrow class_traits::construct method");


    return NESTL_CALL_CLASS_TRAITS_CONSTRUCT;

#undef NESTL_CALL_CLASS_TRAITS_CONSTRUCT
}



template<typename OperationError, typename T, typename ... Args>
OperationError assign(T& dest, Args&& ... args) noexcept
{
#define NESTL_CALL_CLASS_TRAITS_ASSIGN nestl::class_traits<T>:: template assign<OperationError>(dest, std::forward<Args>(args) ...)

    constexpr bool is_nothrow_op = noexcept(NESTL_CALL_CLASS_TRAITS_ASSIGN);
    static_assert(is_nothrow_op, "User should provide nothrow class_traits::assign method");


    return NESTL_CALL_CLASS_TRAITS_ASSIGN;

#undef NESTL_CALL_CLASS_TRAITS_ASSIGN
}

} // namespace detail

} // namespace nestl

#endif // NESTL_CLASS_TRAITS_HPP