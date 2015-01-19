#ifndef NESTL_ADDRESSOF_HPP
#define NESTL_ADDRESSOF_HPP

#include <nestl/config.hpp>

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

#endif /* NESTL_ADDRESSOF_HPP */
