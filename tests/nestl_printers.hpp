#ifndef NESTL_TESTS_NESTL_PRINTERS_HPP
#define NESTL_TESTS_NESTL_PRINTERS_HPP

#include <nestl/operation_error.hpp>

#include <ostream>


inline std::ostream& operator <<(std::ostream& strm, const nestl::has_exceptions::operation_error& err)
{
    try
    {
        std::rethrow_exception(err.value());
    }
    catch (const std::exception& e)
    {
        strm << "std::exception { message : " << e.what() << " };";
    }
    catch (...)
    {
        strm << "unknown exception";
    }
    return strm;
}

inline std::ostream& operator <<(std::ostream& strm, const nestl::no_exceptions::operation_error& err)
{
    strm << "operation_error {value: " << err.value() << " };";
    return strm;
}

#endif /* NESTL_TESTS_NESTL_PRINTERS_HPP */
