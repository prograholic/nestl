#ifndef NESTL_VECTOR_HPP
#define NESTL_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/vector.hpp>
#include <nestl/no_exceptions/vector.hpp>

namespace nestl
{

template <typename T, typename Allocator = nestl::allocator<T>>
using vector = exception_support::dispatch<has_exceptions::vector<T, Allocator>, no_exceptions::vector<T, Allocator>>;

} // namespace nestl

#endif /* NESTL_VECTOR_HPP */
