#ifndef NESTL_NO_EXCEPTIONS_SET_HPP
#define NESTL_NO_EXCEPTIONS_SET_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/set.hpp>

namespace nestl
{
namespace no_exceptions
{

template<typename Key, typename Compare = std::less<Key>, typename Alloc = nestl::allocator<Key> >
using set = impl::set<Key, Compare, Alloc>;

} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_SET_HPP */
