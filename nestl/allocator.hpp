#ifndef NESTL_ALLOCATOR_HPP
#define NESTL_ALLOCATOR_HPP

#include <nestl/config.hpp>
#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/allocator.hpp>
#include <nestl/no_exceptions/allocator.hpp>

namespace nestl
{

template <typename T>
using allocator = exception_support::dispatch<has_exceptions::allocator<T>, no_exceptions::allocator<T>>;

} // namespace nestl

#endif /* NESTL_ALLOCATOR_HPP */
