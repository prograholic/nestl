#ifndef NESTL_MEMORY_HPP
#define NESTL_MEMORY_HPP

#include <nestl/config.hpp>

#include <nestl/class_traits.hpp>

#include <memory>

namespace nestl
{


template <typename Allocator>
struct allocator_traits
{
    typedef Allocator                           allocator_type;
    typedef typename allocator_type::value_type value_type;


/**
 * @note This macro allows to use nested type of allocator (if it has corresponding type)
 * Otherwise it declares type usign default type
 *
 * @note This macro is taken from stdlibc++
 */
#define NESTL_ALLOC_DECLARE_NESTED_TYPE(NestedType, DefaultType) \
  private: \
  template<typename T> \
    static typename T::NestedType NESTL_##NestedType##_helper(T*); \
  static DefaultType NESTL_##NestedType##_helper(...); \
    typedef decltype(NESTL_##NestedType##_helper((Allocator*)0)) nestl_nested_type_##NestedType; \
  public:



#undef NESTL_ALLOC_DECLARE_NESTED_TYPE

};



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
