#ifndef NESTL_TESTS_ALLOCATORS_HPP
#define NESTL_TESTS_ALLOCATORS_HPP

#include <nestl/allocator.hpp>
#include <nestl/shared_ptr.hpp>
#include <nestl/move.hpp>
#include <nestl/set.hpp>

#include <gtest/gtest.h>


namespace nestl
{

namespace test
{

/**
 * @brief Minimal allocator implementation
 */
template <typename T>
class minimal_allocator
{
public:

    typedef T value_type;

    minimal_allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    minimal_allocator(const minimal_allocator& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    template <typename Y>
    minimal_allocator(const minimal_allocator<Y>& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    T* allocate(nestl::size_t n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        return static_cast<T*>(::operator new(n * sizeof(value_type), std::nothrow));
    }

    void deallocate(T* p, nestl::size_t /* n */) NESTL_NOEXCEPT_SPEC
    {
        ::operator delete(p);
    }
};


/**
 * Allocator which always fail (return zero)
 */
template <typename T>
class zero_allocator
{
public:

    typedef T               value_type;

    zero_allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    zero_allocator(const zero_allocator& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    template <typename Y>
    zero_allocator(const zero_allocator<Y>& /* other */) NESTL_NOEXCEPT_SPEC
    {
    }

    ~zero_allocator() NESTL_NOEXCEPT_SPEC
    {
    }

    T* allocate(nestl::size_t /* n */, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        return 0;
    }

    void deallocate(T* p, nestl::size_t /* n */) NESTL_NOEXCEPT_SPEC
    {
        ::operator delete(p);
    }
};


namespace detail
{

struct alloc_impl: private nestl::set<void*>
{
    typedef nestl::set<void*> base_type;
    
    typedef typename base_type::value_type     value_type;
    typedef typename base_type::const_iterator const_iterator;
    
    const_iterator get_end() const noexcept
    {
        return this->end();
    }
    
    void do_erase(const_iterator pos) noexcept
    {
        this->erase(pos);
    }
    
    const_iterator do_find(const value_type& val) const noexcept
    {
        return this->find(val);
    }
    
    void do_insert(const value_type& val) noexcept
    {
        typename base_type::operation_error error;
        this->insert_nothrow(error, val);
        if (error)
        {
            ADD_FAILURE() << "insert failed";
        }
    }
    
    ~alloc_impl()
    {
        if (!this->empty())
        {
            ADD_FAILURE() << "memory leaks detected";
        }
    }
};
    
} // namespace detail

/**
 * Allocator which has its own state (remember who allocate memory)
 */
template <typename T>
class allocator_with_state
{
public:

    typedef T               value_type;

    allocator_with_state() NESTL_NOEXCEPT_SPEC
        : m_allocated_storage(create_storage())
    {
    }

    allocator_with_state(const allocator_with_state& other) NESTL_NOEXCEPT_SPEC
        : m_allocated_storage(other.m_allocated_storage)
    {
    }

    template <typename Y>
    allocator_with_state(const allocator_with_state<Y>& other) NESTL_NOEXCEPT_SPEC
        : m_allocated_storage(other.m_allocated_storage)
    {
    }

    allocator_with_state(allocator_with_state&& other) NESTL_NOEXCEPT_SPEC
        : m_allocated_storage(create_storage())
    {
        swap(m_allocated_storage, other.m_allocated_storage);
    }

    template <typename Y>
    allocator_with_state(allocator_with_state<Y>&& other) NESTL_NOEXCEPT_SPEC
        : m_allocated_storage(create_storage())
    {
        swap(m_allocated_storage, other.m_allocated_storage);
    }

    allocator_with_state& operator=(const allocator_with_state& other) NESTL_NOEXCEPT_SPEC
    {
        m_allocated_storage = other.m_allocated_storage;
        return *this;
    }

    template <typename Y>
    allocator_with_state& operator=(const allocator_with_state<Y>& other) NESTL_NOEXCEPT_SPEC
    {
        m_allocated_storage = other.m_allocated_storage;
        return *this;
    }

    allocator_with_state& operator=(allocator_with_state&& other) NESTL_NOEXCEPT_SPEC
    {
        m_allocated_storage = create_storage();
        swap(m_allocated_storage, other.m_allocated_storage);
        return *this;
    }

    template <typename Y>
    allocator_with_state& operator=(allocator_with_state<Y>&& other) NESTL_NOEXCEPT_SPEC
    {
        m_allocated_storage = create_storage();
        swap(m_allocated_storage, other.m_allocated_storage);
        return *this;
    }

    ~allocator_with_state() NESTL_NOEXCEPT_SPEC
    {
    }

    T* allocate(nestl::size_t n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        T* res = static_cast<T*>(::operator new(n * sizeof(value_type), std::nothrow));


        detail::alloc_impl::const_iterator pos = m_allocated_storage->do_find(res);
        if (pos != m_allocated_storage->get_end())
        {
            ADD_FAILURE() << "pointer [" << static_cast<void*>(res) << "] already belong to this allocator";

            /**
             * @note Maybe it`s a error in test. Do we need assert here ???
             */
        }

        m_allocated_storage->do_insert(res);

        return res;
    }

    void deallocate(T* p, nestl::size_t /* n */) NESTL_NOEXCEPT_SPEC
    {
        detail::alloc_impl::const_iterator pos = m_allocated_storage->do_find(p);
        if (pos == m_allocated_storage->get_end())
        {
            if (p)
            {
                ADD_FAILURE() << "pointer [" << static_cast<void*>(p) << "] does not belong to this allocator";

                /**
                 * @note possible memory leak here, because we do not know how to deallocate
                 */
            }
        }
        else
        {
            m_allocated_storage->do_erase(pos);
            ::operator delete(p);
        }
    }
    
    
    nestl::shared_ptr<detail::alloc_impl> m_allocated_storage;
    
private:
    static nestl::shared_ptr<detail::alloc_impl> create_storage()
    {
        nestl::shared_ptr<detail::alloc_impl> storage;
        nestl::error_condition err = nestl::make_shared(storage);
        if (err)
        {
            ADD_FAILURE() << "make_shared failed";
        }
        
        return storage;
    }
};


template <typename T>
bool operator == (const allocator_with_state<T>& left, const allocator_with_state<T>& right)
{
    return left.m_allocated_storage == right.m_allocated_storage;
}

template <typename T>
bool operator != (const allocator_with_state<T>& left, const allocator_with_state<T>& right)
{
    return left.m_allocated_storage != right.m_allocated_storage;
}

} // namespace test

} // namespace nestl



#endif /* NESTL_TESTS_ALLOCATORS_HPP */
