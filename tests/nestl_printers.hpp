#ifndef NESTL_TESTS_NESTL_PRINTERS_HPP
#define NESTL_TESTS_NESTL_PRINTERS_HPP

#include <nestl/default_operation_error.hpp>

#include <nestl/implementation/list.hpp>

#include <ostream>

namespace nestl
{

#if NESTL_HAS_EXCEPTIONS == 1

namespace has_exceptions
{

inline
std::ostream& operator <<(std::ostream& strm, const exception_ptr_error& err)
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

} // namespace has_exceptions

#endif /* NESTL_HAS_EXCEPTIONS */


namespace no_exceptions
{

inline
std::ostream& operator <<(std::ostream& strm, const errc_based_error& err)
{
    strm << "operation_error {value: " << err.value() << " };";
    return strm;
}

} // namespace no_exceptions




namespace impl
{

template <typename T>
std::ostream& operator <<(std::ostream& strm, const list_iterator<T>& it)
{
    strm << "impl::list_iterator<T> {node pointer: " << static_cast<const void*>(it.get_list_node()) << " };";
    return strm;
}

} // namespace impl

} // namespace nestl

#endif /* NESTL_TESTS_NESTL_PRINTERS_HPP */
