#ifndef NESTL_DECLVAL_HPP
#define NESTL_DECLVAL_HPP

#include <nestl/config.hpp>

#if defined(NESTL_CONFIG_HAS_STD) && defined(NESTL_CONFIG_HAS_CXX11)

#include <utility>

namespace nestl
{

using std::declval

} // namespace nestl



#else /* defined(NESTL_CONFIG_HAS_STD) && defined(NESTL_CONFIG_HAS_CXX11) */

#include <nestl/type_traits.hpp>

namespace nestl
{
template <typename T>
#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
nestl::add_rvalue_reference<T>
#else /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */
T
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */
declval();

} // namespace nestl

#endif /* defined(NESTL_CONFIG_HAS_STD) && defined(NESTL_CONFIG_HAS_CXX11) */




#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
