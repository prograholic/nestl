#ifndef NESTL_DETAIL_CONSTRUCT_HPP
#define NESTL_DETAIL_CONSTRUCT_HPP

#include <nestl/config.hpp>

#include <nestl/addressof.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/addressof.hpp>
#include <nestl/forward.hpp>

namespace nestl
{

namespace detail
{

template <typename Allocator, typename ForwardIterator>
void destroy(Allocator& alloc, ForwardIterator first, ForwardIterator last) NESTL_NOEXCEPT_SPEC
{
    while (first != last)
    {
         nestl::allocator_traits<Allocator>::destroy(alloc, nestl::addressof(*first));
        ++first;
    }
}


template <typename ForwardIterator, typename Allocator>
struct destruction_scoped_guard : private nestl::noncopyable
{
    destruction_scoped_guard(ForwardIterator first, ForwardIterator& last, Allocator& alloc) NESTL_NOEXCEPT_SPEC
        : m_first(first)
        , m_last(&last)
        , m_alloc(alloc)
    {
    }

    ~destruction_scoped_guard() NESTL_NOEXCEPT_SPEC
    {
        nestl::detail::destroy(m_alloc, m_first, *m_last);
    }


    void release()
    {
        m_last = &m_first;
    }

private:
    ForwardIterator m_first;
    ForwardIterator* m_last;
    Allocator& m_alloc;

    destruction_scoped_guard(const destruction_scoped_guard& );
    destruction_scoped_guard& operator =(const destruction_scoped_guard& );
};


template<typename PointerType, typename Allocator>
struct allocation_scoped_guard : private nestl::noncopyable
{
    allocation_scoped_guard(Allocator& alloc, PointerType ptr, size_t size) NESTL_NOEXCEPT_SPEC
        : m_alloc(alloc)
        , m_ptr(ptr)
        , m_size(size)
    {
    }

    ~allocation_scoped_guard() NESTL_NOEXCEPT_SPEC
    {
        nestl::allocator_traits<Allocator>::deallocate(m_alloc, m_ptr, m_size);
    }

    void release()
    {
        m_ptr = 0;
    }

private:
    Allocator& m_alloc;
    PointerType m_ptr;
    size_t m_size;

    allocation_scoped_guard(const allocation_scoped_guard& );
    allocation_scoped_guard& operator =(const allocation_scoped_guard& );
};

}

} // namespace nestl

#endif // NESTL_DETAIL_CONSTRUCT_HPP
