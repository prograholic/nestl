#ifndef NESTL_OPERATION_ERROR_HPP
#define NESTL_OPERATION_ERROR_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>
//#include <nestl/string.hpp>

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

};


namespace error_info
{

template <typename OperationError>
int value(const OperationError& e) noexcept
{
    return nestl::operation_error_traits<OperationError>::value(e);
}


} // namespace error_info


template <typename OperationError, typename Enum>
OperationError make_operation_error(Enum value)
{
    return OperationError{value};
}

} // namespace nestl

#endif // NESTL_OPERATION_ERROR_HPP
