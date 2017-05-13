#ifndef NESTL_DETAIL_MEMORY_HPP
#define NESTL_DETAIL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/allocator_traits.hpp>


#include <type_traits>
#include <cstddef>

namespace nestl
{
namespace detail
{

template <typename Type>
void destroy(Type* val)
{
    val->~Type();
}

template <typename ForwardIterator>
void destroy(ForwardIterator first, ForwardIterator last) NESTL_NOEXCEPT_SPEC
{
    while (first != last)
    {
        destroy(std::addressof(*first));
        ++first;
    }
}

template <typename ForwardIterator>
struct destruction_scoped_guard
{
    destruction_scoped_guard(ForwardIterator first, ForwardIterator& last) NESTL_NOEXCEPT_SPEC
        : m_first(first)
        , m_last(&last)
    {
    }

    ~destruction_scoped_guard() NESTL_NOEXCEPT_SPEC
    {
        nestl::detail::destroy(m_first, *m_last);
    }


    void release()
    {
        m_last = &m_first;
    }

private:
    ForwardIterator m_first;
    ForwardIterator* m_last;

    destruction_scoped_guard(const destruction_scoped_guard&) = delete;
    destruction_scoped_guard& operator =(const destruction_scoped_guard&) = delete;
};


template<typename PointerType, typename Allocator>
struct deallocation_scoped_guard
{
    deallocation_scoped_guard(Allocator& alloc, PointerType ptr, std::size_t size) NESTL_NOEXCEPT_SPEC
        : m_alloc(alloc)
        , m_ptr(ptr)
        , m_size(size)
    {
    }

    ~deallocation_scoped_guard() NESTL_NOEXCEPT_SPEC
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
    std::size_t m_size;

    deallocation_scoped_guard(const deallocation_scoped_guard&) = delete;
    deallocation_scoped_guard& operator =(const deallocation_scoped_guard&) = delete;
};


} // namespace detail
} // namespace nestl

#endif /* NESTL_DETAIL_MEMORY_HPP */
