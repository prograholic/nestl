#ifndef NESTL_ALIGNMENT_HPP
#define NESTL_ALIGNMENT_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>

#if defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES)

namespace nestl
{

using std::aligned_storage;
using std::alignment_of;

} // namespace nestl


#else /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

namespace nestl
{

template<size_t len, size_t alignment>
struct aligned_storage
{
    struct type
    {
        NESTL_ALIGNAS(unsigned char data[len], alignment);
    };
};


template<class T>
struct alignment_of : public nestl::integral_constant<size_t, NESTL_ALIGNOF(T)>
{
};

} // namespace nestl

#endif /* defined(NESTL_CONFIG_HAS_CXX11) && defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_ALIGNMENT_HPP */
