#ifndef NESTL_SYSTEM_ERROR_HPP
#define NESTL_SYSTEM_ERROR_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#   include <system_error>

namespace nestl
{


using std::errc;

using std::error_condition;

} // namespace nestl


#else /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#   include <cerrno>

#   define NESTL_ENOMEM ENOMEM
#   define NESTL_EINVAL EINVAL

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#   define NESTL_ENOMEM 12
#   define NESTL_EINVAL 22

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

namespace nestl
{

namespace errc
{

enum
{
    invalid_argument  = NESTL_EINVAL,
    not_enough_memory = NESTL_ENOMEM
};

} // namespace errc

class error_condition
{
public:

    error_condition()
        : m_value(0)
    {
    }

    explicit error_condition(int val)
        : m_value(val)
    {
    }

    const char* message() const NESTL_NOEXCEPT_SPEC
    {
        return "error";
    }

    int value() const NESTL_NOEXCEPT_SPEC
    {
        return m_value;
    }

#if defined(NESTL_CONFIG_HAS_EXPLICIT_OPERATOR)
    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return m_value != 0;
    }
#else /* defined(NESTL_CONFIG_HAS_EXPLICIT_OPERATOR) */
    typedef int error_condition::*unspecified_bool_type;
    operator unspecified_bool_type() const NESTL_NOEXCEPT_SPEC
    {
        return m_value == 0 ? 0 : &error_condition::m_value;
    }
#endif /* defined(NESTL_CONFIG_HAS_EXPLICIT_OPERATOR) */
private:
    int m_value;
};

}


#endif /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_SYSTEM_ERROR_HPP */
