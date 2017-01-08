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
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
  }
} // namespace nestl

#endif /* NESTL_SWAP_HPP */
