#ifndef NESTL_DEFAULT_OPERATION_ERROR_HPP
#define NESTL_DEFAULT_OPERATION_ERROR_HPP

#include <nestl/config.hpp>
#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/default_operation_error.hpp>
#include <nestl/no_exceptions/default_operation_error.hpp>

namespace nestl
{

using default_operation_error = exception_support::dispatch<has_exceptions::default_operation_error,
                                                            no_exceptions::default_operation_error>;

} // namespace nestl

#endif /* NESTL_DEFAULT_OPERATION_ERROR_HPP */
