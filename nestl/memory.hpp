#ifndef NESTL_MEMORY_HPP
#define NESTL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/class_traits.hpp>
#include <nestl/detail/construct.hpp>
#include <nestl/system_error.hpp>




namespace nestl
{


template <typename InputIterator, typename ForwardIterator, typename Allocator>
error_condition uninitialised_copy(InputIterator first,
                                   InputIterator last,
                                   ForwardIterator output,
                                   Allocator& alloc) NESTL_NOEXCEPT_SPEC
{
    ForwardIterator cur = output;
    nestl::detail::destruction_scoped_guard<ForwardIterator, Allocator> guard(output, cur, alloc);

    for ( ; first != last; ++first, ++cur)
    {
		error_condition ec = nestl::detail::construct<error_condition>(std::addressof(*cur), alloc, *first);
        if (ec)
        {
            return ec;
        }
    }

    guard.release();
    return error_condition();
}

} // namespace nestl

#endif // NESTL_MEMORY_HPP
