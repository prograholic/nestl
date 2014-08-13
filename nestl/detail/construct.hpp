#ifndef NESTL_DETAIL_CONSTRUCT_HPP
#define NESTL_DETAIL_CONSTRUCT_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>
#include <nestl/noncopyable.hpp>

#include <utility>

namespace nestl
{

namespace detail
{

template <typename Allocator, typename ForwardIterator>
void destroy(Allocator& alloc, ForwardIterator first, ForwardIterator last) noexcept
{
    while (first != last)
    {
        alloc.destroy(std::addressof(*first));
        ++first;
    }
}


template <typename ForwardIterator, typename Allocator>
struct destruction_scoped_guard
{
    destruction_scoped_guard(ForwardIterator first, ForwardIterator& last, Allocator& alloc) noexcept
        : m_first(first)
        , m_last(&last)
        , m_alloc(alloc)
    {
    }

    ~destruction_scoped_guard() noexcept
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

};


template<typename PointerType, typename Allocator>
struct allocation_scoped_guard : private nestl::noncopyable
{
    allocation_scoped_guard(Allocator& alloc, PointerType ptr, size_t size) noexcept
        : m_alloc(alloc)
        , m_ptr(ptr)
        , m_size(size)
    {
    }

    ~allocation_scoped_guard() noexcept
    {
        m_alloc.deallocate(m_ptr, m_size);
    }

    void release()
    {
        m_ptr = 0;
    }

private:
    Allocator& m_alloc;
    PointerType m_ptr;
    size_t m_size;
};

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct_impl(T* ptr, Allocator& alloc, Args&& ... args) noexcept
{
    alloc.construct(ptr, std::forward<Args>(args) ...);

    return OperationError();
}

}

} // namespace nestl

#endif // NESTL_DETAIL_CONSTRUCT_HPP
