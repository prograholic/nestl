#ifndef NESTL_OPERATION_ERROR_HPP
#define NESTL_OPERATION_ERROR_HPP

#include <nestl/config.hpp>

#include <type_traits>

namespace nestl
{


/**
 * @brief Default implementation for std::error_condition and std::error_code
 */
template <typename OperationError>
struct operation_error_traits
{

    static int value(const OperationError& e) noexcept
    {
        return e.value();
    }

    static std::string message(const OperationError& e)
    {
        return e.message();
    }
};


namespace error_info
{

template <typename OperationError>
int value(const OperationError& e) noexcept
{
    return nestl::operation_error_traits<OperationError>::value(e);
}

template <typename OperationError>
std::string message(const OperationError& e)
{
    return nestl::operation_error_traits<OperationError>::message(e);
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

    result_with_operation_error(const Type& val, const OperationError& err) noexcept
        : m_val(val)
        , m_error(err)
    {
    }

    const OperationError& error() const noexcept
    {
        return m_error;
    }

    const Type& result() const noexcept
    {
        return m_val;
    }

    explicit operator bool() const noexcept
    {
        return m_error ? true : false;
    }

private:
    Type m_val;
    OperationError m_error;
};


template <typename Type, typename OperationError>
result_with_operation_error<Type, OperationError>
make_result_with_operation_error(const Type& val, const OperationError& err) noexcept
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

    static int value(const operation_error& e) noexcept
    {
        return e.error().value();
    }

    static std::string message(const operation_error& e)
    {
        return e.error().message();
    }
};


} // namespace nestl

#endif // NESTL_OPERATION_ERROR_HPP
