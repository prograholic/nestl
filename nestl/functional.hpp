#ifndef NESTL_FUNCTIONAL_HPP
#define NESTL_FUNCTIONAL_HPP

#include <nestl/config.hpp>

namespace nestl
{

template <typename Arg, typename Result>
struct unary_function
{
    typedef Arg    argument_type;
    typedef Result result_type;
};


template<typename Arg1, typename Arg2, typename Result>
struct binary_function
{
    typedef Arg1   first_argument_type;
    typedef Arg2   second_argument_type;
    typedef Result result_type;
};

template<typename T>
struct less : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const
    {
        return x < y;
    }
};


template<typename T>
struct equal_to : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const
    {
        return x == y;
    }
};

} // namespace nestl

namespace nestl
{

namespace detail
{

template <typename T>
struct identity : public nestl::unary_function<T, T>
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
