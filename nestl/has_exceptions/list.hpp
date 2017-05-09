#ifndef NESTL_HAS_EXCEPTIONS_LIST_HPP
#define NESTL_HAS_EXCEPTIONS_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/list.hpp>

namespace nestl
{
namespace has_exceptions
{

template <typename T, typename Allocator = nestl::allocator<T> >
using list = impl::list<T, Allocator>;

} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_LIST_HPP */
