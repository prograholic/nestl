#ifndef NESTL_HAS_EXCEPTIONS_OPERATION_ERROR_HPP
#define NESTL_HAS_EXCEPTIONS_OPERATION_ERROR_HPP

#include <nestl/config.hpp>

#include <exception>
#include <stdexcept>

namespace nestl
{

namespace has_exceptions
{

class operation_error
{
public:

    operation_error()
        : m_exception()
    {
    }

    explicit operation_error(::std::exception_ptr val)
        : m_exception(val)
    {
    }

    ::std::exception_ptr value() const NESTL_NOEXCEPT_SPEC
    {
        return m_exception;
    }

    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return m_exception != 0;
    }

private:
    ::std::exception_ptr m_exception;
};


inline
void
build_length_error(operation_error& op)
{
    op = operation_error(std::make_exception_ptr(std::length_error("length error")));
}

inline
void
build_bad_alloc(operation_error& op)
{
    op = operation_error(std::make_exception_ptr(std::bad_alloc()));
}

} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_OPERATION_ERROR_HPP */
