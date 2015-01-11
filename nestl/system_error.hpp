#ifndef NESTL_SYSTEM_ERROR_HPP
#define NESTL_SYSTEM_ERROR_HPP

#include <nestl/config.hpp>

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

#endif /* NESTL_SYSTEM_ERROR_HPP */
