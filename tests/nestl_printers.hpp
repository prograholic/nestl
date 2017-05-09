#ifndef NESTL_TESTS_NESTL_PRINTERS_HPP
#define NESTL_TESTS_NESTL_PRINTERS_HPP

#include <nestl/default_operation_error.hpp>

#include <ostream>

#if NESTL_HAS_EXCEPTIONS

inline std::ostream& operator <<(std::ostream& strm, const nestl::has_exceptions::exception_ptr_error& err)
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

#endif /* NESTL_HAS_EXCEPTIONS */

inline std::ostream& operator <<(std::ostream& strm, const nestl::no_exceptions::errc_based_error& err)
{
    strm << "operation_error {value: " << err.value() << " };";
    return strm;
}

#endif /* NESTL_TESTS_NESTL_PRINTERS_HPP */
