#ifndef NESTL_ALLOCATOR_HPP
#define NESTL_ALLOCATOR_HPP

#include <nestl/config.hpp>

#include <nestl/memory.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/assert.hpp>
#include <nestl/numeric_limits.hpp>
#include <nestl/new.hpp>
#include <nestl/exception_support.hpp>


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
    
    typedef can_deal_with_exceptions<value_type> has_exceptions;
    typedef can_deal_with_noexcept<value_type> has_noexcept;

    template<typename U>
    struct rebind
    {
        typedef allocator<U> other;
    };

    allocator() noexcept
    {
    }

    allocator(const allocator& /* other */) noexcept
    {
    }

    template <typename Y>
    allocator(const allocator<Y>& /* other */) noexcept
    {
    }

    ~allocator() noexcept
    {
    }

    pointer address(reference x) const noexcept
    {
        return nestl::addressof(x);
    }

    const_pointer address(const_reference x) const noexcept
    {
        return nestl::addressof(x);
    }

    typename enable_if<has_exceptions, pointer>::type
    allocate(size_type n, const void* /* hint */ = 0)
    {
        return static_cast<pointer>(::operator new(n * sizeof(value_type)));
    }
    
    typename enable_if<has_noexcept, pointer>::type
    allocate_nothrow(size_type n, const void* /* hint */ = 0) noexcept
    {
        return static_cast<pointer>(::operator new(n * sizeof(value_type), std::nothrow));
    }

    void deallocate(pointer p, size_type /* n */) noexcept
    {
        ::operator delete(p);
    }

    size_type max_size() const noexcept
    {
        return nestl::numeric_limits<size_type>::max();
    }

    template<typename U, typename ... Args>
    void construct(U* ptr, Args&& ... args) noexcept
    {
        NESTL_ASSERT(ptr);
        ::new(static_cast<void*>(ptr)) U(nestl::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* ptr) noexcept
    {
        NESTL_ASSERT(ptr);
        ptr->~U();
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_HPP */
