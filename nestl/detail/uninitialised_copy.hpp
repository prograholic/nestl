#ifndef NESTL_MEMORY_HPP
#define NESTL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/class_operations.hpp>

#include <nestl/detail/destroy.hpp>

namespace nestl
{
namespace detail
{

template <typename OperationError, typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialised_copy(OperationError& err,
                                   InputIterator first,
                                   InputIterator last,
                                   ForwardIterator output) NESTL_NOEXCEPT_SPEC
{
    ForwardIterator cur = output;
    nestl::detail::destruction_scoped_guard<ForwardIterator> guard(output, cur);

    for ( ; first != last; ++first, ++cur)
    {
		nestl::class_operations::construct(err, std::addressof(*cur), *first);
        if (err)
        {
            return cur;
        }
    }

    guard.release();
    return cur;
}

} // namespace detail
} // namespace nestl

#endif // NESTL_MEMORY_HPP
