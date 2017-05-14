#ifndef NESTL_NO_EXCEPTIONS_DETAIL_CLASS_OPERATIONS_HPP
#define NESTL_NO_EXCEPTIONS_DETAIL_CLASS_OPERATIONS_HPP

#include <nestl/config.hpp>

#include <new>
#include <utility>

namespace nestl
{
namespace no_exceptions
{
namespace detail
{

struct class_operations_helper
{
    template <typename T, typename OperationError, typename ... Args>
    static
        void construct(OperationError& err, T* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        ::new(static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
    }


    template <typename T, typename OperationError, typename Y>
    static
        void assign(OperationError& /* err */, T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
    {
        dest = std::forward<Y>(src);
    }
};



} // namespace detail
} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_DETAIL_CLASS_OPERATIONS_HPP */
