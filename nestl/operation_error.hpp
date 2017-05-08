#ifndef NESTL_OPERATION_ERROR_HPP
#define NESTL_OPERATION_ERROR_HPP

#include <nestl/config.hpp>
#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/operation_error.hpp>
#include <nestl/no_exceptions/operation_error.hpp>

namespace nestl
{

using operation_error = exception_support::dispatch<has_exceptions::operation_error, no_exceptions::operation_error>;

}

#endif /* NESTL_OPERATION_ERROR_HPP */
