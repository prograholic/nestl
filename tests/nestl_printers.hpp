#ifndef NESTL_TESTS_NESTL_PRINTERS_HPP
#define NESTL_TESTS_NESTL_PRINTERS_HPP

#include <system_error>
#include <ostream>

#include <nestl/noncopyable.hpp>
#include <nestl/operation_error.hpp>

inline std::ostream& operator <<(std::ostream& strm, const std::error_condition& ec)
{
    strm << "error_condition { message : " << ec.message() << ", value: " << ec.value() << " };";
    return strm;
}


template <typename Result, typename OperationError>
inline std::ostream& operator <<(std::ostream& strm, const nestl::result_with_operation_error<Result, OperationError>& ec)
{
    strm << "result_with_operation_error { message : " << ec.error().message() << ", value: " << ec.error().value() << " };";
    return strm;
}


#endif /* NESTL_TESTS_NESTL_PRINTERS_HPP */
