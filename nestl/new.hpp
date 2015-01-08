#ifndef NESTL_NEW_HPP
#define NESTL_NEW_HPP

#include <nestl/config.hpp>
#include <nestl/cstddef.hpp>

#if defined(NESTL_CONFIG_HAS_STD_INCLUDES)

#   include <new>

#else /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

/**
 * @note We declare nothrow_t and nothrow in std namespace
 * so we can link with standard libraries even without STD support
 */
namespace std
{

struct nothrow_t {};

extern nothrow_t nothrow;

class bad_alloc
{
};

} // namespace std


void* operator new(nestl::size_t) NESTL_THROW_SPEC(std::bad_alloc);

void* operator new[](nestl::size_t) NESTL_THROW_SPEC(std::bad_alloc);

void operator delete(void*) NESTL_NOEXCEPT_OR_THROW_SPEC;

void operator delete[](void*) NESTL_NOEXCEPT_OR_THROW_SPEC;

void* operator new(nestl::size_t, const std::nothrow_t&) NESTL_NOEXCEPT_SPEC;

void* operator new[](nestl::size_t, const std::nothrow_t&) NESTL_NOEXCEPT_SPEC;

void operator delete(void*, const std::nothrow_t&) NESTL_NOEXCEPT_SPEC;

void operator delete[](void*, const std::nothrow_t&) NESTL_NOEXCEPT_SPEC;


// Default placement versions of operator new.
inline void* operator new(nestl::size_t, void* location) NESTL_NOEXCEPT_SPEC
{
    return location;
}
inline void* operator new[](nestl::size_t, void* location) NESTL_NOEXCEPT_SPEC
{
    return location;
}

// Default placement versions of operator delete.
inline void operator delete  (void*, void*) NESTL_NOEXCEPT_SPEC
{
}

inline void operator delete[](void*, void*) NESTL_NOEXCEPT_SPEC
{
}

#endif /* defined(NESTL_CONFIG_HAS_STD_INCLUDES) */

#endif /* NESTL_NEW_HPP */
