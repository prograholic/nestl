#ifndef NESTL_CHAR_TRAITS_HPP
#define NESTL_CHAR_TRAITS_HPP

#include <nestl/config.hpp>

#if defined(NESTL_USE_STD)

#include <string>

namespace nestl
{

using std::char_traits;

} // namespace nestl

#else /* NESTL_USE_STD */

namespace nestl
{

template <typename CharType>
struct char_traits
{
    typedef CharType char_type;
};

} // namespace nestl

#endif /* NESTL_USE_STD */

#endif /* NESTL_CHAR_TRAITS_HPP */
