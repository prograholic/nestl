#ifndef NESTL_HAS_EXCEPTIONS_SET_HPP
#define NESTL_HAS_EXCEPTIONS_SET_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/set.hpp>

namespace nestl
{
namespace has_exceptions
{

template<typename Key, typename Compare = std::less<Key>, typename Alloc = nestl::allocator<Key> >
using set = impl::set<Key, Compare, Alloc>;

} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_SET_HPP */
