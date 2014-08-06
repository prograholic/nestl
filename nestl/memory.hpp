#ifndef NESTL_MEMORY_HPP
#define NESTL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/detail/construct.hpp>

#include <memory>

namespace nestl
{

namespace detail
{

template <typename ForwardIterator, typename Allocator>
struct uninitialised_copy_guard
{
    uninitialised_copy_guard(ForwardIterator first, ForwardIterator& last, Allocator& alloc) noexcept
        : m_first(first)
        , m_last(last)
        , m_alloc(alloc)
    {
    }

    ~uninitialised_copy_guard() noexcept
    {
        nestl::detail::destroy(m_alloc, m_first, m_last);
    }

    ForwardIterator m_first;
    ForwardIterator& m_last;
    Allocator& m_alloc;

};

} // namespace detail

template <typename OperationError, typename InputIterator, typename ForwardIterator, typename Allocator>
OperationError uninitialised_copy(InputIterator first, InputIterator last, ForwardIterator output, Allocator& alloc) noexcept
{
    ForwardIterator cur = output;
    detail::uninitialised_copy_guard<ForwardIterator, Allocator> guard(output, cur, alloc);

    for ( ; first != last; ++first, ++cur)
    {
        OperationError err = nestl::detail::construct<OperationError>(std::addressof(*cur), alloc, *first);
        if (err)
        {
            return err;
        }
    }

    /// disable destruction
    guard.m_last = guard.m_first;

    return OperationError();
}

} // namespace nestl

#endif // NESTL_MEMORY_HPP
