#ifndef NESTL_ALIGNMENT_HPP
#define NESTL_ALIGNMENT_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>

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

namespace nestl
{

template <typename T>
struct aligned_buffer : public nestl::aligned_storage<sizeof(T), nestl::alignment_of<T>::value>
{
    typedef typename nestl::aligned_storage<sizeof(T), nestl::alignment_of<T>::value>::type storage_type;

    storage_type m_storage;

    void* address() NESTL_NOEXCEPT_SPEC
    {
        return static_cast<void*>(&m_storage);
    }

    const void* address() const NESTL_NOEXCEPT_SPEC
    {
        return static_cast<const void*>(&m_storage);
    }

    T* ptr() NESTL_NOEXCEPT_SPEC
    {
        return static_cast<T*>(address());
    }

    const T* ptr() const NESTL_NOEXCEPT_SPEC
    {
        return static_cast<const T*>(address());
    }

};

}

#endif /* NESTL_ALIGNMENT_HPP */
