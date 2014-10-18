#ifndef NESTL_CLASS_TRAITS_HPP
#define NESTL_CLASS_TRAITS_HPP

#include <nestl/config.hpp>

#include <nestl/static_assert.hpp>

#include <nestl/detail/construct.hpp>

/**
 * @file implementation of class_traits template
 */

namespace nestl
{

template <typename T>
struct class_traits
{

#if NESTL_HAS_VARIADIC_TEMPLATES

    template <typename OperationError, typename Allocator, typename ... Args>
    static OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        return nestl::detail::construct_impl<OperationError>(ptr, alloc, std::forward<Args>(args) ...);
    }

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

    template <typename OperationError, typename Allocator, typename Arg>
    static OperationError construct(T* ptr, Allocator& alloc, const Arg& arg) NESTL_NOEXCEPT_SPEC
    {
        return nestl::detail::construct_impl<OperationError>(ptr, alloc, arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */



#if NESTL_HAS_RVALUE_REF

    template <typename OperationError, typename Y>
    static OperationError assign(T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
        dest = std::forward<Y>(src);

        return OperationError();
    }

#else /* NESTL_HAS_RVALUE_REF */

    template <typename OperationError, typename Y>
    static OperationError assign(T& dest, const Y& src) NESTL_NOEXCEPT_SPEC
    {
        dest = src;

        return OperationError();
    }

#endif /* NESTL_HAS_RVALUE_REF */

};


namespace detail
{

#if NESTL_HAS_VARIADIC_TEMPLATES

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct(T* ptr, Allocator& alloc, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
#define NESTL_CALL_CLASS_TRAITS_CONSTRUCT \
    nestl::class_traits<T>:: template construct<OperationError>(ptr, alloc, std::forward<Args>(args) ...)

    NESTL_STATIC_ASSERT(NESTL_NOEXCEPT_OPERATOR(NESTL_CALL_CLASS_TRAITS_CONSTRUCT),
                        "User should provide nothrow class_traits::construct method");


    return NESTL_CALL_CLASS_TRAITS_CONSTRUCT;

#undef NESTL_CALL_CLASS_TRAITS_CONSTRUCT
}

template<typename OperationError, typename T, typename ... Args>
OperationError assign(T& dest, Args&& ... args) noexcept
{
#define NESTL_CALL_CLASS_TRAITS_ASSIGN \
    nestl::class_traits<T>:: template assign<OperationError>(dest, std::forward<Args>(args) ...)

    constexpr bool is_nothrow_op = NESTL_NOEXCEPT_OPERATOR(NESTL_CALL_CLASS_TRAITS_ASSIGN);
    static_assert(is_nothrow_op, "User should provide nothrow class_traits::assign method");


    return NESTL_CALL_CLASS_TRAITS_ASSIGN;

#undef NESTL_CALL_CLASS_TRAITS_ASSIGN
}

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

template<typename OperationError, typename T, typename Allocator, typename Arg>
OperationError construct(T* ptr, Allocator& alloc, const Arg& arg) NESTL_NOEXCEPT_SPEC
{
#define NESTL_CALL_CLASS_TRAITS_CONSTRUCT \
    nestl::class_traits<T>:: template construct<OperationError>(ptr, alloc, arg)

    NESTL_STATIC_ASSERT(NESTL_NOEXCEPT_OPERATOR(NESTL_CALL_CLASS_TRAITS_CONSTRUCT),
                        "User should provide nothrow class_traits::construct method");


    return NESTL_CALL_CLASS_TRAITS_CONSTRUCT;

#undef NESTL_CALL_CLASS_TRAITS_CONSTRUCT
}

template<typename OperationError, typename T, typename Arg>
OperationError assign(T& dest, const Arg& arg) NESTL_NOEXCEPT_SPEC
{
#define NESTL_CALL_CLASS_TRAITS_ASSIGN \
    nestl::class_traits<T>:: template assign<OperationError>(dest, arg)

    NESTL_STATIC_ASSERT(NESTL_NOEXCEPT_OPERATOR(NESTL_CALL_CLASS_TRAITS_ASSIGN),
                        "User should provide nothrow class_traits::assign method");


    return NESTL_CALL_CLASS_TRAITS_ASSIGN;

#undef NESTL_CALL_CLASS_TRAITS_ASSIGN
}

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */




} // namespace detail

} // namespace nestl

#endif // NESTL_CLASS_TRAITS_HPP
