#ifndef NESTL_MEMORY_HPP
#define NESTL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/class_operations.hpp>
#include <nestl/detail/construct.hpp>


namespace nestl
{


template <typename InputIterator, typename ForwardIterator, typename Allocator>
ForwardIterator uninitialised_copy(typename Allocator::operation_error& err,
                                   InputIterator first,
                                   InputIterator last,
                                   ForwardIterator output,
                                   Allocator& alloc) NESTL_NOEXCEPT_SPEC
{
    ForwardIterator cur = output;
    nestl::detail::destruction_scoped_guard<ForwardIterator, Allocator> guard(output, cur, alloc);

    for ( ; first != last; ++first, ++cur)
    {
		nestl::detail::construct(err, std::addressof(*cur), alloc, *first);
        if (err)
        {
            return cur;
        }
    }

    guard.release();
    return cur;
}

} // namespace nestl

#endif // NESTL_MEMORY_HPP
