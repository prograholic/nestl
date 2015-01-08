#ifndef NESTL_INITIALIZER_LIST_HPP
#define NESTL_INITIALIZER_LIST_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_INITIALIZER_LIST)

#include <initializer_list>

namespace nestl
{

using std::initializer_list;

} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_INITIALIZER_LIST) */

namespace nestl
{

/**
 * @note initializer_list requires compiler support
 */

} // namespace nestl

#endif /* defined(NESTL_CONFIG_HAS_INITIALIZER_LIST) */



#endif /* NESTL_INITIALIZER_LIST_HPP */
