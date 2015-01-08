#ifndef NESTL_FUNCTIONAL_HPP
#define NESTL_FUNCTIONAL_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#   include <functional>

namespace nestl
{

using std::less;

} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */


namespace nestl
{


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


#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_FUNCTIONAL_HPP */
