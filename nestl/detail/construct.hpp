#ifndef NESTL_DETAIL_CONSTRUCT_HPP
#define NESTL_DETAIL_CONSTRUCT_HPP

#include <nestl/config.hpp>

#include <nestl/type_traits.hpp>
#include <nestl/memory.hpp>

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

struct has_nothrow_constructor
{
};

struct has_assign_copy_member
{
};

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct_with_tag(const has_nothrow_constructor&, T* ptr, Allocator& alloc, Args&& ... args)
{
    alloc.construct(ptr, std::move<Args>(args) ...);

    return OperationError();
}

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct_with_tag(const has_assign_copy_member&, T* ptr, Allocator& alloc, Args&& ... args)
{
    alloc.construct(ptr);

    OperationError err = ptr->assign_copy(std::forward<Args>(args) ...);
    if (err)
    {
        /// @todo Should we destroy ptr?
        return err;
    }

    return err;
}

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct(T* ptr, Allocator& alloc, Args&& ... args)
{
    typedef typename std::conditional
    <
        std::is_nothrow_move_constructible<T>::value || std::is_nothrow_copy_constructible<T>::value,
        has_nothrow_constructor,
        typename std::conditional
        <
            std::is_nothrow_default_constructible<T>::value && nestl::has_assign_copy_member<T>::value,
            has_assign_copy_member,
            void
        >::type
    >::type construction_tag;

    return construct_with_tag<OperationError>(construction_tag(), ptr, alloc, std::forward<Args>(args) ...);
}

} // namespace detail

} // namespace nestl

#endif // NESTL_DETAIL_CONSTRUCT_HPP
