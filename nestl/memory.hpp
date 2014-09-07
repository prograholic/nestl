#ifndef NESTL_MEMORY_HPP
#define NESTL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/class_traits.hpp>

#include <memory>

namespace nestl
{

template <typename OperationError, typename InputIterator, typename ForwardIterator, typename Allocator>
OperationError uninitialised_copy(InputIterator first, InputIterator last, ForwardIterator output, Allocator& alloc) noexcept
{
    ForwardIterator cur = output;
    nestl::detail::destruction_scoped_guard<ForwardIterator, Allocator> guard(output, cur, alloc);

    for ( ; first != last; ++first, ++cur)
    {
        OperationError err = nestl::detail::construct<OperationError>(std::addressof(*cur), alloc, *first);
        if (err)
        {
            return err;
        }
    }

    guard.release();
    return OperationError();
}

} // namespace nestl

#endif // NESTL_MEMORY_HPP
