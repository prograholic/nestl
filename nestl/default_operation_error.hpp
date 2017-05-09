#ifndef NESTL_DEFAULT_OPERATION_ERROR_HPP
#define NESTL_DEFAULT_OPERATION_ERROR_HPP

#include <nestl/config.hpp>
#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/exception_ptr_error.hpp>
#include <nestl/no_exceptions/errc_based_error.hpp>

namespace nestl
{

using default_operation_error = exception_support::dispatch<has_exceptions::exception_ptr_error, no_exceptions::errc_based_error>;

}

#endif /* NESTL_DEFAULT_OPERATION_ERROR_HPP */
