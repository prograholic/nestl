#ifndef NESTL_OPERATION_ERROR_HPP
#define NESTL_OPERATION_ERROR_HPP

#include <nestl/config.hpp>

namespace nestl
{


/**
 * @brief Default implementation for std::error_condition and std::error_code
 */
template <typename OperationError>
struct operation_error_traits
{

    static int value(const OperationError& e) NESTL_NOEXCEPT_SPEC
    {
        return e.value();
    }

};


namespace error_info
{

template <typename OperationError>
int value(const OperationError& e) NESTL_NOEXCEPT_SPEC
{
    return nestl::operation_error_traits<OperationError>::value(e);
}


} // namespace error_info


template <typename Type, typename OperationError>
class result_with_operation_error
{
public:

    result_with_operation_error()
        : m_val()
        , m_error()
    {
    }

    result_with_operation_error(const Type& val, const OperationError& err) NESTL_NOEXCEPT_SPEC
        : m_val(val)
        , m_error(err)
    {
    }

    template <typename ErrorCode>
    result_with_operation_error(const Type& val, const ErrorCode& ec) NESTL_NOEXCEPT_SPEC
        : m_val(val)
        , m_error(ec)
    {
    }

    const OperationError& error() const NESTL_NOEXCEPT_SPEC
    {
        return m_error;
    }

    const Type& result() const NESTL_NOEXCEPT_SPEC
    {
        return m_val;
    }

    Type& result() NESTL_NOEXCEPT_SPEC
    {
        return m_val;
    }

    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return m_error ? true : false;
    }

private:
    Type m_val;
    OperationError m_error;
};


template <typename Type, typename OperationError>
result_with_operation_error<Type, OperationError>
make_result_with_operation_error(const Type& val, const OperationError& err) NESTL_NOEXCEPT_SPEC
{
    return result_with_operation_error<Type, OperationError>(val, err);
}

/**
 * @brief Specialization for result_with_operation_error
 */
template <typename Type, typename OperationError>
struct operation_error_traits <result_with_operation_error<Type, OperationError> >
{

    typedef result_with_operation_error<Type, OperationError> operation_error;

    static int value(const operation_error& e) NESTL_NOEXCEPT_SPEC
    {
        return e.error().value();
    }
};


} // namespace nestl

#endif // NESTL_OPERATION_ERROR_HPP
