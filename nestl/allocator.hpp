#ifndef NESTL_ALLOCATOR_HPP
#define NESTL_ALLOCATOR_HPP

#include <nestl/config.hpp>

#include <nestl/memory.hpp>

#include <type_traits>
#include <limits>
#include <cassert>
#include <new>

namespace nestl
{

template <typename T>
class allocator
{
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;

    typedef std::true_type  propagate_on_container_move_assignment;

    template<typename U>
    struct rebind
    {
        typedef allocator<U> other;
    };

    allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    allocator(const allocator& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    template <typename Y>
    allocator(const allocator<Y>& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    ~allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    pointer address(reference x) const NESTL_NOEXCEPT_SPEC
    {
        return std::addressof(x);
    }

    const_pointer address(const_reference x) const NESTL_NOEXCEPT_SPEC
    {
        return std::addressof(x);
    }

    pointer allocate(size_type n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        return static_cast<pointer>(::operator new(n * sizeof(value_type), std::nothrow));
    }

    void deallocate(pointer p, size_type /* n */) NESTL_NOEXCEPT_SPEC
    {
        ::operator delete(p);
    }

    size_type max_size() const NESTL_NOEXCEPT_SPEC
    {
        return std::numeric_limits<size_type>::max();
    }

    template<typename U, typename ... Args>
    void construct(U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* ptr)
    {
        assert(ptr);
        ptr->~U();
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_HPP */
