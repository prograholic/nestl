#ifndef NESTL_TESTS_ALLOCATORS_HPP
#define NESTL_TESTS_ALLOCATORS_HPP

#include <nestl/allocator.hpp>
#include <nestl/shared_ptr.hpp>
#include <nestl/set.hpp>

#include "tests/test_common.hpp"

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

    template <typename OperationError>
    T* allocate(OperationError& err, std::size_t n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        auto res = static_cast<T*>(::operator new(n * sizeof(value_type), std::nothrow));
        if (!res)
        {
            build_bad_alloc(err);
        }
        
        return res;
    }

    void deallocate(T* p, std::size_t /* n */) NESTL_NOEXCEPT_SPEC
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

    template <typename OperationError>
    T* allocate(OperationError& err, std::size_t n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        build_bad_alloc(err);

        return nullptr;
    }

    void deallocate(T* p, std::size_t /* n */) NESTL_NOEXCEPT_SPEC
    {
        ::operator delete(p);
    }
};


/**
 * Allocator which has its own state (remember who allocate memory)
 */
template <typename T>
class allocator_with_state
{
public:

    typedef T               value_type;

    allocator_with_state() NESTL_NOEXCEPT_SPEC
        : m_allocated_storage()
    {
        NESTL_CHECK_OPERATION(m_allocated_storage = nestl::make_shared_nothrow<storage_t>(_));
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
        : m_allocated_storage(std::move(other.m_allocated_storage))
    {
        assert(!other.m_allocated_storage);
    }

    template <typename Y>
    allocator_with_state(allocator_with_state<Y>&& other) NESTL_NOEXCEPT_SPEC
        : m_allocated_storage(std::move(other.m_allocated_storage))
    {
        assert(!other.m_allocated_storage);
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
		m_allocated_storage = std::move(other.m_allocated_storage);
        assert(!other.m_allocated_storage);
        return *this;
    }

    template <typename Y>
    allocator_with_state& operator=(allocator_with_state<Y>&& other) NESTL_NOEXCEPT_SPEC
    {
        m_allocated_storage = std::move(other.m_allocated_storage);
        assert(!other.m_allocated_storage);
        return *this;
    }

    ~allocator_with_state() NESTL_NOEXCEPT_SPEC
    {
        if (m_allocated_storage && !m_allocated_storage->empty())
        {
            fatal_failure("memory leaks detected!!!");
        }
    }

    template <typename OperationError>
    T* allocate(OperationError& err, std::size_t n, const void* /* hint */ = 0) NESTL_NOEXCEPT_SPEC
    {
        auto res = static_cast<T*>(::operator new(n * sizeof(value_type), std::nothrow));
        if (!res)
        {
            build_bad_alloc(err);
            return nullptr;
        }

        nestl::set<void*>::const_iterator pos = m_allocated_storage->find(res);
        if (pos != m_allocated_storage->end())
        {
            fatal_failure("pointer [", static_cast<void*>(res), "] already belong to this allocator");
        }

        NESTL_CHECK_OPERATION(m_allocated_storage->insert_nothrow(_, res));

        return res;
    }

    void deallocate(T* p, std::size_t /* n */) NESTL_NOEXCEPT_SPEC
    {
        nestl::set<void*>::const_iterator pos = m_allocated_storage->find(p);
        if (pos == m_allocated_storage->end())
        {
            if (p)
            {
                fatal_failure("pointer [", static_cast<void*>(p), "] does not belong to this allocator");

                /**
                 * @note possible memory leak here, because we do not know how to deallocate
                 */
            }
        }
        else
        {
            m_allocated_storage->erase(pos);
            ::operator delete(p);
        }
    }

    typedef nestl::set<void*> storage_t;
    nestl::shared_ptr<storage_t> m_allocated_storage;
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
