#ifndef NESTL_FUNCTIONAL_HPP
#define NESTL_FUNCTIONAL_HPP

#include <nestl/config.hpp>

#if NESTL_USE_STD

#   include <functional>

namespace nestl
{

using std::less;
using std::unary_function;

} // namespace nestl

#else /* NESTL_USE_STD */


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

} // namespace nestl


#endif /* NESTL_USE_STD */


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
