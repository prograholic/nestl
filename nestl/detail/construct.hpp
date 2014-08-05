#ifndef NESTL_DETAIL_CONSTRUCT_HPP
#define NESTL_DETAIL_CONSTRUCT_HPP

#include <nestl/config.hpp>

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

} // namespace detail

} // namespace nestl

#endif // NESTL_DETAIL_CONSTRUCT_HPP
