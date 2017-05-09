#ifndef NESTL_NO_EXCEPTIONS_SHARED_PTR
#define NESTL_NO_EXCEPTIONS_SHARED_PTR

#include <nestl/config.hpp>

#include <nestl/implementation/shared_ptr.hpp>

namespace nestl
{
namespace no_exceptions
{


template <typename T>
using weak_ptr = impl::weak_ptr<T>;

template <typename T>
using shared_ptr = impl::shared_ptr<T>;

using impl::make_shared_a_nothrow;
using impl::make_shared_nothrow;

} // namespace has_exceptions
} // namespace nestl


#endif /* NESTL_NO_EXCEPTIONS_SHARED_PTR */
