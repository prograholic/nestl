#ifndef NESTL_NO_EXCEPTIONS_OPERATION_ERROR_HPP
#define NESTL_NO_EXCEPTIONS_OPERATION_ERROR_HPP

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


namespace no_exceptions
{

class operation_error
{
public:

    operation_error()
        : m_value(0)
    {
    }

    explicit operation_error(int val)
        : m_value(val)
    {
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

inline
void
build_length_error(operation_error& op)
{
    op = operation_error(errc::value_too_large);
}

inline
void
build_bad_alloc(operation_error& op)
{
    op = operation_error(errc::not_enough_memory);
}

} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_OPERATION_ERROR_HPP */
