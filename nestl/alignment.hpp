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
        alignas(alignment) unsigned char data[len];
    };
};


template<class T>
struct alignment_of : public nestl::integral_constant<size_t, alignof(T)>
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

    void* address() noexcept
    {
        return static_cast<void*>(&m_storage);
    }

    const void* address() const noexcept
    {
        return static_cast<const void*>(&m_storage);
    }

    T* ptr() noexcept
    {
        return static_cast<T*>(address());
    }

    const T* ptr() const noexcept
    {
        return static_cast<const T*>(address());
    }

};

}

#endif /* NESTL_ALIGNMENT_HPP */
