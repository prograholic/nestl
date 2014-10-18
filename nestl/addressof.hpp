#ifndef NESTL_ADDRESSOF_HPP
#define NESTL_ADDRESSOF_HPP

#include <nestl/config.hpp>

#if NESTL_ENABLE_CXX11

#include <memory>

namespace nestl
{

using std::addressof;

} // namespace nestl

#else /* NESTL_ENABLE_CXX11 */

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


#endif /* NESTL_ENABLE_CXX11 */


#endif /* NESTL_ADDRESSOF_HPP */
