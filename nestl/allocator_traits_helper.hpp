#ifndef NESTL_ALLOCATOR_TRAITS_HELPER_HPP
#define NESTL_ALLOCATOR_TRAITS_HELPER_HPP

#include <nestl/config.hpp>

#include <nestl/detail/select_type.hpp>

namespace nestl
{

template <template <typename, typename ...> class Alloc, typename T1, typename T2, typename ... Args>
struct allocator_rebind_helper
{
    typedef Alloc<T2, Args...> T2Alloc;
    typedef Alloc<T1, Args...> T1Alloc;

    NESTL_SELECT_NESTED_TYPE_TEMPLATE(T2Alloc, rebind<T1>::other, other, T1Alloc);

    typedef nestl_nested_type_other other;
};

template <typename Alloc, typename T>
struct allocator_rebind;


template <template <typename, typename ...> class Alloc, typename T, typename U, typename ... Args>
struct allocator_rebind<Alloc<U, Args...>, T>
{
    typedef typename allocator_rebind_helper<Alloc, T, U, Args...>::other other;
};

namespace detail
{

template <typename Alloc>
void alloc_on_move(Alloc& src, Alloc& dst, std::true_type) NESTL_NOEXCEPT_SPEC
{
    /// @todo provide move for c++-03 version

    src = NESTL_MOVE_IF_SUPPORTED(dst);
}

template <typename Alloc>
void alloc_on_move(Alloc& /* src */, Alloc& /* dst */, std::false_type) NESTL_NOEXCEPT_SPEC
{
}

template <typename Alloc>
void alloc_on_move(Alloc& src, Alloc& dst) NESTL_NOEXCEPT_SPEC
{
    typedef nestl::allocator_traits<Alloc> alloc_traits;
    typedef typename alloc_traits::propagate_on_container_move_assignment move_required;
    alloc_on_move(src, dst, move_required());
}


template <typename Alloc, typename OperationError>
OperationError alloc_on_copy(Alloc& src, const Alloc& dst, std::true_type) NESTL_NOEXCEPT_SPEC
{
    return nestl::detail::assign(src, dst);
}

template <typename Alloc, typename OperationError>
OperationError alloc_on_copy(Alloc& /* src */, const Alloc& /* dst */, std::false_type) NESTL_NOEXCEPT_SPEC
{
    return OperationError();
}

template <typename Alloc, typename OperationError>
void alloc_on_copy(Alloc& src, const Alloc& dst) NESTL_NOEXCEPT_SPEC
{
    typedef nestl::allocator_traits<Alloc> alloc_traits;
    typedef typename alloc_traits::propagate_on_container_copy_assignment copy_requred;

    return alloc_on_copy(src, dst, copy_requred());
}

} // namespace detail

} // namespace nestl


#endif /* NESTL_ALLOCATOR_TRAITS_HELPER_HPP */
