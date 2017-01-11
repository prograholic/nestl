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

    const char* message() const noexcept
    {
        return "error";
    }

    int value() const noexcept
    {
        return m_value;
    }

    explicit operator bool() const noexcept
    {
        return m_value != 0;
    }

private:
    int m_value;
};

}

#endif /* NESTL_SYSTEM_ERROR_HPP */
