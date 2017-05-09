#ifndef NESTL_NO_EXCEPTIONS_LIST_HPP
#define NESTL_NO_EXCEPTIONS_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/list.hpp>

namespace nestl
{
namespace no_exceptions
{

template <typename T, typename Allocator = nestl::allocator<T>>
using list  = impl::list<T, Allocator>;

} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_LIST_HPP */
