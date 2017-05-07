#ifndef NESTL_FUNCTIONAL_HPP
#define NESTL_FUNCTIONAL_HPP

#include <nestl/config.hpp>

#include <functional>

namespace nestl
{
namespace detail
{

template <typename T>
struct identity : public std::unary_function<T, T>
{
    T& operator()(T& x) const
    {
        return x;
    }

    const T& operator()(const T& x) const
    {
        return x;
    }
};

} // namespace detail
} // namespace nestl

#endif /* NESTL_FUNCTIONAL_HPP */
