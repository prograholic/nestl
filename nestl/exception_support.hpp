#ifndef NESTL_EXCEPTION_SUPPORT_HPP
#define NESTL_EXCEPTION_SUPPORT_HPP

#include <nestl/config.hpp>

#include <type_traits>

namespace nestl
{

struct exception_support_t : public std::integral_constant<bool, NESTL_HAS_EXCEPTIONS>
{
};

namespace exception_support
{

template <typename HasExceptions, typename NoExceptions>
using dispatch = typename ::std::conditional<exception_support_t::value, HasExceptions, NoExceptions>::type;

} // namespace exception_support
}

#endif /* NESTL_EXCEPTION_SUPPORT_HPP */
