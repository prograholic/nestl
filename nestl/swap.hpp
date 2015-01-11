#ifndef NESTL_SWAP_HPP
#define NESTL_SWAP_HPP

#include <nestl/config.hpp>

#include <nestl/move.hpp>

namespace nestl
{

template<typename T>
inline void
swap(T& a, T& b)
{
    T tmp = NESTL_MOVE_IF_SUPPORTED(a);
    a = NESTL_MOVE_IF_SUPPORTED(b);
    b = NESTL_MOVE_IF_SUPPORTED(tmp);
  }
} // namespace nestl

#endif /* NESTL_SWAP_HPP */
