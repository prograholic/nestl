#ifndef NESTL_TESTS_ALLOCATORS_HPP
#define NESTL_TESTS_ALLOCATORS_HPP

#include <nestl/allocator.hpp>

#include <set>
#include <memory>

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

    minimal_allocator() noexcept
    {
    }

    minimal_allocator(const minimal_allocator& other) noexcept
    {
    }

    template <typename Y>
    minimal_allocator(const minimal_allocator<Y>& other) noexcept
    {
    }

    T* allocate(std::size_t n, const void* /* hint */ = 0) noexcept
    {
        return static_cast<T*>(::operator new(n * sizeof(value_type), std::nothrow));
    }

    void deallocate(T* p, std::size_t /* n */) noexcept
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

    zero_allocator() noexcept
    {
    }

    zero_allocator(const zero_allocator& other) noexcept
    {
    }

    template <typename Y>
    zero_allocator(const zero_allocator<Y>& other) noexcept
    {
    }

    ~zero_allocator() noexcept
    {
    }

    T* allocate(std::size_t n, const void* /* hint */ = 0) noexcept
    {
        return 0;
    }

    void deallocate(T* p, std::size_t /* n */) noexcept
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

    allocator_with_state() noexcept
        : m_allocated_storage(std::make_shared<std::set<void*> >())
    {
    }

    allocator_with_state(const allocator_with_state& other) noexcept
        : m_allocated_storage(other.m_allocated_storage)
    {
    }

    template <typename Y>
    allocator_with_state(const allocator_with_state<Y>& other) noexcept
        : m_allocated_storage(other.m_allocated_storage)
    {
    }

    ~allocator_with_state() noexcept
    {
        if (!m_allocated_storage->empty())
        {
            ADD_FAILURE() << "memory leaks detected!!!";
        }
    }

    T* allocate(std::size_t n, const void* /* hint */ = 0) noexcept
    {
        T* res = static_cast<T*>(::operator new(n * sizeof(value_type), std::nothrow));


        auto pos = m_allocated_storage->find(res);
        if (pos == m_allocated_storage->end())
        {
            ADD_FAILURE() << "pointer [" << static_cast<void*>(res) << "] already belong to this allocator";

            /**
             * @note Maybe it`s a error in test. Do we need assert here ???
             */
        }

        m_allocated_storage->insert(res);

        return res;
    }

    void deallocate(T* p, std::size_t /* n */) noexcept
    {
        auto pos = m_allocated_storage->find(p);
        if (pos == m_allocated_storage->end())
        {
            ADD_FAILURE() << "pointer [" << static_cast<void*>(p) << "] does not belong to this allocator";

            /**
             * @note possible memory leak here, because we do not know how to deallocate
             */
        }
        else
        {
            m_allocated_storage->erase(pos);

            ::operator delete(p);
        }
    }


private:

    std::shared_ptr<std::set<void*> > m_allocated_storage;
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
