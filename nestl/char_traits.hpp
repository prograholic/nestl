#ifndef NESTL_CHAR_TRAITS_HPP
#define NESTL_CHAR_TRAITS_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#include <string>

namespace nestl
{

using std::char_traits;

} // namespace nestl

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

namespace nestl
{

template <typename CharType>
struct char_traits
{
    typedef CharType char_type;
};

} // namespace nestl

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_CHAR_TRAITS_HPP */
