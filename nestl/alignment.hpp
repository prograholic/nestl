#ifndef NESTL_ALIGNMENT_HPP
#define NESTL_ALIGNMENT_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>

#if NESTL_ENABLE_CXX11 && NESTL_USE_STD

namespace nestl
{

using std::aligned_storage;
using std::alignment_of;

} // namespace nestl


#else /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */

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

#endif /* NESTL_ENABLE_CXX11 && NESTL_USE_STD */

#endif /* NESTL_ALIGNMENT_HPP */
