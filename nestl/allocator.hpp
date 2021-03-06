#ifndef NESTL_ALLOCATOR_HPP
#define NESTL_ALLOCATOR_HPP

#include <nestl/config.hpp>


#include <type_traits>
#include <limits>
#include <new>

namespace nestl
{

template <typename T>
class allocator
{
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;

    typedef std::true_type  propagate_on_container_move_assignment;

    template<typename U>
    struct rebind
    {
        typedef allocator<U> other;
    };

    allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    allocator(const allocator& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    template <typename Y>
    allocator(const allocator<Y>& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    ~allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    template<typename OperationError>
    pointer allocate(OperationError& err, size_type n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        pointer res = static_cast<pointer>(::operator new(n * sizeof(value_type), std::nothrow));
        if (res == nullptr)
        {
            build_bad_alloc(err);
        }

        return res;
    }

    void deallocate(pointer p, size_type /* n */) NESTL_NOEXCEPT_SPEC
    {
        ::operator delete(p);
    }
};

} // namespace nestl

#endif /* NESTL_ALLOCATOR_HPP */
