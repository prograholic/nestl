#ifndef NESTL_ALLOCATOR_HPP
#define NESTL_ALLOCATOR_HPP

#include <nestl/config.hpp>

#include <nestl/memory.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/assert.hpp>
#include <nestl/numeric_limits.hpp>
#include <nestl/new.hpp>


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
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

    typedef nestl::true_type  propagate_on_container_move_assignment;

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
        return nestl::addressof(x);
    }

    const_pointer address(const_reference x) const NESTL_NOEXCEPT_SPEC
    {
        return nestl::addressof(x);
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
        return nestl::numeric_limits<size_type>::max_value();
    }

#if NESTL_HAS_VARIADIC_TEMPLATES

    template<typename U, typename ... Args>
    void construct(U* ptr, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(nestl::forward<Args>(args)...);
    }

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

    template<typename U>
    void construct(U* ptr) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U();
    }

    template<typename U, typename Arg>
    void construct(U* ptr, const Arg& arg) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(arg);
    }

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */

    template<typename U>
    void destroy(U* ptr)
    {
        NESTL_ASSERT(ptr);
        ptr->~U();
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_HPP */
