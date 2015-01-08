#ifndef NESTL_SET_HPP
#define NESTL_SET_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)
#   include <set>

namespace nestl
{

using std::set;

#define NESTL_CONFIG_HAS_SET 1

} // nestl_not_implemented_yet

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#include <nestl/functional.hpp>

namespace nestl
{

template<typename Key, typename Compare = nestl::less<Key>, typename Alloc = nestl::allocator<Key> >
class set;

#define NESTL_CONFIG_HAS_SET 0

} // namespace nestl

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_SET_HPP */
