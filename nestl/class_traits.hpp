#ifndef NESTL_CLASS_TRAITS_HPP
#define NESTL_CLASS_TRAITS_HPP

#include <nestl/config.hpp>

#include <nestl/static_assert.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/forward.hpp>

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
        nestl::allocator_traits<Allocator>::construct(alloc, ptr, nestl::forward<Args>(args) ...);

        return OperationError();
    }

    template <typename OperationError, typename Y>
    static OperationError assign(T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
        dest = nestl::forward<Y>(src);

        return OperationError();
    }
};


namespace detail
{

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
#define NESTL_CALL_CLASS_TRAITS_CONSTRUCT \
    nestl::class_traits<T>:: template construct<OperationError>(ptr, alloc, nestl::forward<Args>(args) ...)

    NESTL_STATIC_ASSERT(NESTL_NOEXCEPT_OPERATOR(NESTL_CALL_CLASS_TRAITS_CONSTRUCT),
                        "User should provide nothrow class_traits::construct method");


    return NESTL_CALL_CLASS_TRAITS_CONSTRUCT;

#undef NESTL_CALL_CLASS_TRAITS_CONSTRUCT
}

template<typename OperationError, typename T, typename ... Args>
OperationError assign(T& dest, Args&& ... args) noexcept
{
#define NESTL_CALL_CLASS_TRAITS_ASSIGN \
    nestl::class_traits<T>:: template assign<OperationError>(dest, nestl::forward<Args>(args) ...)

    NESTL_CONSTEXPR bool is_nothrow_op = NESTL_NOEXCEPT_OPERATOR(NESTL_CALL_CLASS_TRAITS_ASSIGN);
    NESTL_STATIC_ASSERT(is_nothrow_op, "User should provide nothrow class_traits::assign method");


    return NESTL_CALL_CLASS_TRAITS_ASSIGN;

#undef NESTL_CALL_CLASS_TRAITS_ASSIGN
}


} // namespace detail

} // namespace nestl

#endif // NESTL_CLASS_TRAITS_HPP
