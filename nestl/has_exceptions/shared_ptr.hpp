#ifndef NESTL_HAS_EXCEPTIONS_SHARED_PTR
#define NESTL_HAS_EXCEPTIONS_SHARED_PTR

#include <nestl/config.hpp>

#include <nestl/implementation/shared_ptr.hpp>

namespace nestl
{
namespace has_exceptions
{


template <typename T>
using weak_ptr = impl::weak_ptr<T>;

template <typename T>
using shared_ptr = impl::shared_ptr<T>;


} // namespace has_exceptions
} // namespace nestl


#endif /* NESTL_HAS_EXCEPTIONS_SHARED_PTR */
