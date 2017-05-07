#ifndef NESTL_SYSTEM_ERROR_HPP
#define NESTL_SYSTEM_ERROR_HPP

#include <nestl/config.hpp>

#include <cerrno>

namespace nestl
{

namespace errc
{

enum
{
    invalid_argument  = EINVAL,
    not_enough_memory = ENOMEM,
    value_too_large   = EOVERFLOW
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

    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return m_value != 0;
    }

private:
    int m_value;
};

}

#endif /* NESTL_SYSTEM_ERROR_HPP */
