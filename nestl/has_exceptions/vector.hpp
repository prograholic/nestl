#ifndef NESTL_HAS_EXCEPTIONS_VECTOR_HPP
#define NESTL_HAS_EXCEPTIONS_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/vector.hpp>

namespace nestl
{
namespace has_exceptions
{


template <typename T, typename Allocator = nestl::allocator<T> >
using vector = impl::vector<T, Allocator>;

} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_VECTOR_HPP */
