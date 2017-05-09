#ifndef NESTL_HAS_EXCEPTIONS_EXCEPTION_PTR_ERROR_HPP
#define NESTL_HAS_EXCEPTIONS_EXCEPTION_PTR_ERROR_HPP

#include <nestl/config.hpp>

#include <exception>
#include <stdexcept>

namespace nestl
{

namespace has_exceptions
{

class exception_ptr_error
{
public:

    exception_ptr_error()
        : m_exception()
    {
    }

    explicit exception_ptr_error(::std::exception_ptr val)
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
from_exception_ptr(exception_ptr_error& err, std::exception_ptr eptr)
{
    err = exception_ptr_error(eptr);
}

template <typename ExceptionType>
inline
void
from_exception(exception_ptr_error& err, ExceptionType exc)
{
    from_exception_ptr(err, std::make_exception_ptr(exc));
}




inline
void
build_length_error(exception_ptr_error& err)
{
    from_exception(err, std::length_error("length error"));
}

inline
void
build_bad_alloc(exception_ptr_error& err)
{
    from_exception(err, std::bad_alloc());
}



} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_EXCEPTION_PTR_ERROR_HPP */
