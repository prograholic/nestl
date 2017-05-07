#ifndef NESTL_ALIGNMENT_HPP
#define NESTL_ALIGNMENT_HPP

#include <nestl/config.hpp>

#include <type_traits>

namespace nestl
{

template <typename T>
struct aligned_buffer : public std::aligned_storage<sizeof(T), std::alignment_of<T>::value>
{
    typedef typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage_type;

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
