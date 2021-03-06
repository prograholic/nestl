#ifndef NESTL_NO_EXCEPTIONS_VECTOR_HPP
#define NESTL_NO_EXCEPTIONS_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/vector.hpp>

namespace nestl
{
namespace no_exceptions
{


template <typename T, typename Allocator = nestl::allocator<T> >
using vector = impl::vector<T, Allocator>;

} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_VECTOR_HPP */
