#ifndef NESTL_DECLVAL_HPP
#define NESTL_DECLVAL_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>

namespace nestl
{
template <typename T>
#if NESTL_HAS_RVALUE_REF
nestl::add_rvalue_reference<T>
#else /* NESTL_HAS_RVALUE_REF */
T
#endif /* NESTL_HAS_RVALUE_REF */
declval();

} // namespace nestl

#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
