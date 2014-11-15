#ifndef NESTL_SYSTEM_ERROR_HPP
#define NESTL_SYSTEM_ERROR_HPP

#include <nestl/config.hpp>

#if NESTL_ENABLE_CXX11 && NESTL_USE_STD

#   include <system_error>

namespace nestl
{


using std::errc;

using std::error_condition;

} // namespace nestl


#else /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */

#if NESTL_USE_STD

#   include <cerrno>

#   define NESTL_ENOMEM    ENOMEM
#   define NESTL_EINVAL    EINVAL
#   define NESTL_EOVERFLOW EOVERFLOW

#else /* NESTL_USE_STD */

#   define NESTL_ENOMEM    12
#   define NESTL_EINVAL    22
#   define NESTL_EOVERFLOW 75

#endif /* NESTL_USE_STD */

namespace nestl
{

namespace errc
{

enum
{
    invalid_argument  = NESTL_EINVAL,
    not_enough_memory = NESTL_ENOMEM,
    value_too_large   = NESTL_EOVERFLOW
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

#if NESTL_HAS_EXPLICIT_OPERATOR
    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return m_value != 0;
    }
#else /* NESTL_HAS_EXPLICIT_OPERATOR */
    typedef int error_condition::*unspecified_bool_type;
    operator unspecified_bool_type() const NESTL_NOEXCEPT_SPEC
    {
        return m_value == 0 ? 0 : &error_condition::m_value;
    }
#endif /* NESTL_HAS_EXPLICIT_OPERATOR */
private:
    int m_value;
};

}


#endif /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */

#endif /* NESTL_SYSTEM_ERROR_HPP */
