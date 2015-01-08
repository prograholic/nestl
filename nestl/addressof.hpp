#ifndef NESTL_ADDRESSOF_HPP
#define NESTL_ADDRESSOF_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#include <memory>

namespace nestl
{

using std::addressof;

} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

namespace nestl
{

template< class T >
T* addressof(T& arg)
{
    return reinterpret_cast<T*>(
               &const_cast<char&>(
                  reinterpret_cast<const volatile char&>(arg)));
}

} // namespace nestl


#endif /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */


#endif /* NESTL_ADDRESSOF_HPP */
