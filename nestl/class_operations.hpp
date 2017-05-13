#ifndef NESTL_CLASS_OPERATIONS_HPP
#define NESTL_CLASS_OPERATIONS_HPP

#include <nestl/config.hpp>

#include <nestl/exception_support.hpp>
#include <nestl/has_exceptions/detail/class_operations.hpp>
#include <nestl/no_exceptions/detail/class_operations.hpp>

#include <nestl/type_traits.hpp>

#include <nestl/detail/destroy.hpp>

#include <type_traits>

/**
 * @file emulation of various class operations
 */

namespace nestl
{
namespace class_operations
{
namespace detail
{

using class_operations_helper = exception_support::dispatch<has_exceptions::detail::class_operations_helper,
                                                            no_exceptions::detail::class_operations_helper>;

} // namespace detail

template <typename T, typename OperationError, typename ... Args>
typename std::enable_if<std::is_constructible<T, Args...>::value>::type
construct(OperationError& err, T* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    detail::class_operations_helper::construct(err, ptr, std::forward<Args>(args) ...);
}

/// @brief Emulate construction via 2-phase initialization
template <typename T, typename OperationError, typename ... Args>
typename std::enable_if<std::is_default_constructible<T>::value &&
                        !std::is_constructible<T, Args...>::value &&
                        !nestl::two_phase_initializable<T, Args...>::value>::type
construct(OperationError& err, T* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    detail::class_operations_helper::construct(err, ptr); // use default constructor
    if (err)
    {
        return;
    }
    T* end = ptr + 1;
    nestl::detail::destruction_scoped_guard<T*> guard(ptr, end);

    nestl::two_phase_initializable<T, Args...>::init(err, ptr, std::forward<Args>(args)...);
    if (err)
    {
        return;
    }

    guard.release();
}



template <typename T, typename OperationError, typename Y>
typename std::enable_if<std::is_assignable<T&, Y&&>::value>::type
assign(OperationError& err, T& dest, Y&& src) NESTL_NOEXCEPT_SPEC
{
    detail::class_operations_helper::assign(err, dest, std::forward<Y>(src));
}

} // namespace class_operations
} // namespace nestl

#endif // NESTL_CLASS_OPERATIONS_HPP
