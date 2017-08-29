#ifndef NESTL_ALLOCATOR_TRAITS_HPP
#define NESTL_ALLOCATOR_TRAITS_HPP

#include <nestl/config.hpp>
#include <nestl/detail/select_type.hpp>

#include <type_traits>
#include <limits>
#include <utility>

namespace nestl
{

template <typename Allocator>
struct allocator_traits
{
    typedef Allocator                                 allocator_type;
    typedef typename allocator_type::value_type       value_type;

    NESTL_SELECT_NESTED_TYPE(Allocator, pointer, value_type*);
    typedef nestl_nested_type_pointer                 pointer;

    NESTL_SELECT_NESTED_TYPE(Allocator, const_pointer, const value_type*);
    typedef nestl_nested_type_const_pointer           const_pointer;

	NESTL_SELECT_NESTED_TYPE(Allocator, propagate_on_container_move_assignment, std::false_type);
    typedef nestl_nested_type_propagate_on_container_move_assignment propagate_on_container_move_assignment;

	NESTL_SELECT_NESTED_TYPE(Allocator, size_type, std::size_t);
    typedef nestl_nested_type_size_type               size_type;


    /**
     * @note Each allocator should provide method allocate
     */
    template <typename OperationError>
    static pointer allocate(OperationError& err, Allocator& alloc, size_type n, void* hint = 0) NESTL_NOEXCEPT_SPEC
    {
        return alloc.allocate(err, n, hint);
    }

    /**
     * @note Each allocator should provide method deallocate
     */
    template<typename U>
    static void deallocate(Allocator& alloc, U* ptr, size_type n) NESTL_NOEXCEPT_SPEC
    {
        alloc.deallocate(ptr, n);
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_TRAITS_HPP */
