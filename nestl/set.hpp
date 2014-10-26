#ifndef NESTL_SET_HPP
#define NESTL_SET_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD
#   include <set>

namespace nestl
{

using std::set;

#define NESTL_HAS_SET 1

} // nestl_not_implemented_yet

#else /* NESTL_USE_STD */

#include <nestl/functional.hpp>

namespace nestl
{

template<typename Key, typename Compare = nestl::less<Key>, typename Alloc = nestl::allocator<Key> >
class set;

#define NESTL_HAS_SET 0

} // namespace nestl

#endif /* NESTL_USE_STD */

#endif /* NESTL_SET_HPP */
