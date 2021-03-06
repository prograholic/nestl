#ifndef NESTL_HAS_EXCEPTIONS_DETAIL_CLASS_OPERATIONS_HPP
#define NESTL_HAS_EXCEPTIONS_DETAIL_CLASS_OPERATIONS_HPP

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

struct class_operations_helper
{
    template <typename T, typename OperationError, typename ... Args>
    static
    typename std::enable_if<std::is_nothrow_constructible<T, Args...>::value>::type
    construct(OperationError& /* err */, T* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        ::new(static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
    }

    template <typename T, typename OperationError, typename ... Args>
    static
    typename std::enable_if<!std::is_nothrow_constructible<T, Args...>::value>::type
    construct(OperationError& err, T* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        try
        {
		    ::new(static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
        }
        catch (...)
        {
            from_exception_ptr(err, std::current_exception());
        }
    }



    template <typename T, typename OperationError, typename Y>
    static
        typename std::enable_if<std::is_nothrow_assignable<T, Y>::value>::type
        assign(OperationError& /* err */, T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
        dest = std::forward<Y>(src);
    }

    template <typename T, typename OperationError, typename Y>
    static
    typename std::enable_if<!std::is_nothrow_assignable<T, Y>::value>::type
    assign(OperationError& err, T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
        try
        {
            dest = std::forward<Y>(src);
        }
        catch (...)
        {
            from_exception_ptr(err, std::current_exception());
        }
    }
};

} // namespace detail
} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_DETAIL_MEMORY_HPP */
