#ifndef NESTL_SHARED_PTR
#define NESTL_SHARED_PTR

#include <nestl/config.hpp>

#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/shared_ptr.hpp>
#include <nestl/no_exceptions/shared_ptr.hpp>

namespace nestl
{

template <typename T>
using weak_ptr = exception_support::dispatch<has_exceptions::weak_ptr<T>, no_exceptions::weak_ptr<T>>;

template <typename T>
using shared_ptr = exception_support::dispatch<has_exceptions::shared_ptr<T>, no_exceptions::shared_ptr<T>>;

using impl::make_shared_nothrow;
using impl::make_shared_a_nothrow;


} // namespace nestl

#endif /* NESTL_SHARED_PTR */
