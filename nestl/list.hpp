#ifndef NESTL_LIST_HPP
#define NESTL_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/list.hpp>
#include <nestl/no_exceptions/list.hpp>

namespace nestl
{

template <typename T, typename Allocator = nestl::allocator<T>>
using list = exception_support::dispatch<has_exceptions::list<T, Allocator>, no_exceptions::list<T, Allocator>>;

} // namespace nestl

#endif /* NESTL_LIST_HPP */
