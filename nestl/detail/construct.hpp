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
        , m_last(last)
        , m_alloc(alloc)
    {
    }

    ~destruction_scoped_guard() noexcept
    {
        nestl::detail::destroy(m_alloc, m_first, m_last);
    }

    ForwardIterator m_first;
    ForwardIterator& m_last;
    Allocator& m_alloc;

};

struct has_nothrow_constructor
{
};

struct has_nothrow_move_constructor
{
};

struct has_assign_copy_member
{
};

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct_with_tag(const has_nothrow_move_constructor&, T* ptr, Allocator& alloc, Args&& ... args)
{
    alloc.construct(ptr, std::move<Args>(args) ...);

    return OperationError();
}

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct_with_tag(const has_nothrow_constructor&, T* ptr, Allocator& alloc, Args&& ... args)
{
    alloc.construct(ptr, std::forward<Args>(args) ...);

    return OperationError();
}

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct_with_tag(const has_assign_copy_member&, T* ptr, Allocator& alloc, Args&& ... args)
{
    alloc.construct(ptr);

    T* tmp = ptr + 1;
    nestl::detail::destruction_scoped_guard<T*, Allocator> guard(ptr, tmp, alloc);

    OperationError err = ptr->assign_copy(std::forward<Args>(args) ...);
    if (err)
    {
        return err;
    }

    /// disable destruction of object;
    tmp = ptr;

    return err;
}

template<typename OperationError, typename T, typename Allocator, typename ... Args>
OperationError construct(T* ptr, Allocator& alloc, Args&& ... args)
{
    constexpr bool use_move_ctor = (std::is_nothrow_move_constructible<T>::value)
            || ((sizeof...(Args) == 0) && std::is_nothrow_default_constructible<T>::value);

    constexpr bool use_ctor = !std::is_nothrow_move_constructible<T>::value && std::is_nothrow_constructible<T, Args ...>::value;

    constexpr bool use_assign_copy = std::is_nothrow_default_constructible<T>::value && nestl::has_assign_copy_member<T>::value;

    typedef typename std::conditional
    <
        use_move_ctor,
        has_nothrow_move_constructor,
        typename std::conditional
        <
            use_ctor,
            has_nothrow_constructor,
            typename std::conditional
            <
                use_assign_copy,
                has_assign_copy_member,
                void
            >::type
        >::type
    >::type construction_tag;

    return construct_with_tag<OperationError>(construction_tag(), ptr, alloc, std::forward<Args>(args) ...);
}

} // namespace detail

} // namespace nestl

#endif // NESTL_DETAIL_CONSTRUCT_HPP
