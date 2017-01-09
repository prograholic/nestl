#ifndef NESTL_TESTS_NESTL_PRINTERS_HPP
#define NESTL_TESTS_NESTL_PRINTERS_HPP

#include <nestl/system_error.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/operation_error.hpp>

#include <ostream>


inline std::ostream& operator <<(std::ostream& strm, const nestl::error_condition& ec)
{
    strm << "error_condition { message : " << ec.message() << ", value: " << ec.value() << " };";
    return strm;
}

#endif /* NESTL_TESTS_NESTL_PRINTERS_HPP */
