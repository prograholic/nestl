#ifndef NESTL_SET_HPP
#define NESTL_SET_HPP

#include <nestl/config.hpp>

#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/set.hpp>
#include <nestl/no_exceptions/set.hpp>

namespace nestl
{

template<typename Key, typename Compare = std::less<Key>, typename Alloc = nestl::allocator<Key> >
using set = exception_support::dispatch<has_exceptions::set<Key, Compare, Alloc>, no_exceptions::set<Key, Compare, Alloc>>;

} // namespace nestl

#endif /* NESTL_SET_HPP */
