#ifndef NESTL_NO_EXCEPTIONS_ERRC_BASED_ERROR_HPP
#define NESTL_NO_EXCEPTIONS_ERRC_BASED_ERROR_HPP

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

class errc_based_error
{
public:

    errc_based_error() NESTL_NOEXCEPT_SPEC
        : m_value(0)
    {
    }

    explicit errc_based_error(int val) NESTL_NOEXCEPT_SPEC
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
build_length_error(errc_based_error& op)
{
    op = errc_based_error(errc::value_too_large);
}

inline
void
build_bad_alloc(errc_based_error& op)
{
    op = errc_based_error(errc::not_enough_memory);
}

} // namespace no_exceptions
} // namespace nestl

#endif /* NESTL_NO_EXCEPTIONS_ERRC_BASED_ERROR_HPP */
