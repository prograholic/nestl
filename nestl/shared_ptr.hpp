#ifndef NESTL_SHARED_PTR_HPP
#define NESTL_SHARED_PTR_HPP

#include <nestl/config.hpp>

#include <system_error>

#include <nestl/memory.hpp>
#include <nestl/allocator.hpp>
#include <nestl/detail/construct.hpp>


namespace nestl
{

namespace detail
{

class shared_count_base : private nestl::noncopyable
{
public:

    shared_count_base() noexcept
        : m_use_count(0)
        , m_weak_use_count(0)
    {
    }

    virtual ~shared_count_base()
    {
    }

    long use_count() const noexcept
    {
        NESTL_ASSERT(m_use_count >= 0);
        return m_use_count;
    }

    void add_ref() noexcept
    {
        NESTL_ASSERT(m_use_count >= 0);
        ++m_use_count;
    }

    void release() noexcept
    {
        NESTL_ASSERT(m_use_count > 0);
        --m_use_count;
        if (m_use_count == 0)
        {
            destroy_value();
            if (m_weak_use_count == 0)
            {
                destroy_self();
            }
        }
    }

    void weak_add_ref() noexcept
    {
        NESTL_ASSERT(m_weak_use_count >= 0);
        ++m_weak_use_count;
    }

    void weak_release() noexcept
    {
        NESTL_ASSERT(m_weak_use_count > 0);
        --m_weak_use_count;
        if ((m_weak_use_count == 0) && (m_use_count == 0))
        {
            destroy_self();
        }
    }

private:
    long m_use_count;
    long m_weak_use_count;

    virtual void destroy_value() noexcept = 0;

    virtual void destroy_self() noexcept = 0;
};


template <typename T, typename TypeAllocator, typename OperationError>
class type_stored_by_value : public shared_count_base
{
public:

    typedef OperationError operation_error;

    typedef typename TypeAllocator:: template rebind<type_stored_by_value>::other allocator_type;

    type_stored_by_value(const allocator_type& alloc) noexcept
        : shared_count_base()
        , m_value()
        , m_allocator(alloc)
    {
    }

    T* get()
    {
        return reinterpret_cast<T*>(&m_value);
    }

    virtual void destroy_value() noexcept
    {
        T* value = get();
        nestl::detail::destroy(m_allocator, value, value + 1);
    }

    virtual void destroy_self() noexcept
    {
        allocator_type alloc(m_allocator);

        alloc.destroy(this);
        alloc.deallocate(this, 1);
    }

    template <typename ... Args>
    operation_error initialize(Args&& ... args) noexcept
    {
        operation_error err = nestl::detail::construct<operation_error>(get(), m_allocator, std::forward<Args>(args) ...);
        return err;
    }

private:
    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_value;
    allocator_type m_allocator;
};

} // namespace detail

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr
{
public:

    typedef T                    element_type;
    typedef T*                   pointer_type;
    typedef std::error_condition operation_error;

    /// constructors
    shared_ptr() noexcept;

    shared_ptr(const shared_ptr& other) noexcept;

    template <typename Y>
    shared_ptr(const shared_ptr<Y>& other) noexcept;

    shared_ptr(shared_ptr&& other) noexcept;

    /// destructor
    ~shared_ptr() noexcept;

    /// assignment operators
    shared_ptr& operator=(const shared_ptr& other) noexcept;

    template <typename Y>
    shared_ptr& operator=(const shared_ptr<Y>& other) noexcept;

    template <typename Y>
    shared_ptr& operator=(shared_ptr<Y>&& other) noexcept;

    /// assign_copy
    template <typename Y>
    operation_error assign_copy(const shared_ptr<Y>& other) noexcept;

    void reset() noexcept;

    void swap(shared_ptr& other) noexcept;

    /// observers
    element_type* get() const noexcept;

    element_type& operator*() const noexcept;

    element_type* operator->() const noexcept;

    long use_count() const noexcept;

    bool unique() const noexcept;

    explicit operator bool() const noexcept;

private:
    element_type* m_ptr;
    detail::shared_count_base* m_refcount;

    shared_ptr(element_type* ptr, detail::shared_count_base* refcount)
        : m_ptr(ptr)
        , m_refcount(refcount)
    {
        if (m_refcount)
        {
            m_refcount->add_ref();
        }
    }

    template <typename Type>
    friend class weak_ptr;

    template <typename Type, typename Allocator, typename Y, typename ... Args>
    friend
    typename shared_ptr<Type>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& alloc, Args&& ... args);
};


template <typename T>
class weak_ptr
{
public:
    typedef T                    element_type;
    typedef T*                   pointer_type;
    typedef std::error_condition operation_error;

    weak_ptr()
        : m_ptr(0)
        , m_refcount(0)
    {
    }

    weak_ptr(weak_ptr&& other)
        : m_ptr(other.m_ptr)
        , m_refcount(other.m_refcount)
    {
        other.m_ptr = 0;
        other.m_refcount = 0;
    }

    weak_ptr(const shared_ptr<T>& sp) noexcept
        : m_ptr(sp.m_ptr)
        , m_refcount(sp.m_refcount)
    {
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }
    }

    weak_ptr(const weak_ptr& other) noexcept
        : m_ptr(other.m_ptr)
        , m_refcount(other.m_refcount)
    {
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }
    }


    void reset() noexcept
    {
        const weak_ptr tmp(std::move(*this));
    }

    ~weak_ptr()
    {
        if (m_refcount)
        {
            m_refcount->weak_release();
        }
    }

    weak_ptr& operator=(const shared_ptr<T>& other) noexcept
    {
        const weak_ptr tmp(std::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }

        return *this;
    }

    weak_ptr& operator=(const weak_ptr& other) noexcept
    {
        const weak_ptr tmp(std::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }

        return *this;
    }

    weak_ptr& operator=(weak_ptr&& other) noexcept
    {
        const weak_ptr tmp(std::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;

        return *this;
    }

    bool expired() const noexcept
    {
        return use_count() == 0;
    }

    long use_count() const noexcept
    {
        if (m_refcount)
        {
            return (m_refcount)->use_count();
        }

        return 0;
    }

    shared_ptr<T> lock()
    {
        return shared_ptr<T>(m_ptr, m_refcount);
    }

private:
    element_type* m_ptr;
    detail::shared_count_base* m_refcount;

};




/// Implementation

template <typename T>
shared_ptr<T>::shared_ptr() noexcept
    : m_ptr(0)
    , m_refcount(0)
{
}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other) noexcept
    : m_ptr(other.m_ptr)
    , m_refcount(other.m_refcount)
{
    if (m_refcount)
    {
        m_refcount->add_ref();
    }
}

template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(const shared_ptr<Y>& other) noexcept
    : m_ptr(other.m_ptr)
    , m_refcount(other.m_refcount)
{
    if (m_refcount)
    {
        m_refcount->add_ref();
    }
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other) noexcept
    : m_ptr(0)
    , m_refcount(0)
{
    this->swap(other);
}

template <typename T>
shared_ptr<T>::~shared_ptr() noexcept
{
    if (m_refcount)
    {
        m_refcount->release();
    }
}


template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other) noexcept
{
    if (&other != this)
    {
        const shared_ptr tmp(std::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;

        if (m_refcount)
        {
            m_refcount->add_ref();
        }
    }

    return *this;
}

template <typename T>
template <typename Y>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<Y>& other) noexcept
{
    if (&other != this)
    {
        const shared_ptr tmp(std::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;

        if (m_refcount)
        {
            m_refcount->add_ref();
        }
    }

    return *this;
}

template <typename T>
template <typename Y>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<Y>&& other) noexcept
{
    if (&other != this)
    {
        const shared_ptr tmp(std::move(*this));

        this->swap(other);
    }

    return *this;
}

template <typename T>
template <typename Y>
typename shared_ptr<T>::operation_error shared_ptr<T>::assign_copy(const shared_ptr<Y>& other) noexcept
{
    *this = other;
    return operation_error();
}


template <typename T>
void shared_ptr<T>::reset() noexcept
{
    const shared_ptr tmp(std::move(*this));
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr& other) noexcept
{
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_refcount, other.m_refcount);
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::get() const noexcept
{
    return m_ptr;
}

template <typename T>
typename shared_ptr<T>::element_type& shared_ptr<T>::operator*() const noexcept
{
    NESTL_ASSERT(m_ptr);
    return *m_ptr;
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::operator->() const noexcept
{
    NESTL_ASSERT(m_ptr);
    return m_ptr;
}

template <typename T>
long shared_ptr<T>::use_count() const noexcept
{
    return m_refcount ? m_refcount->use_count() : 0;
}

template <typename T>
bool shared_ptr<T>::unique() const noexcept
{
    return (use_count() == 1);
}

template <typename T>
shared_ptr<T>::operator bool() const noexcept
{
    return (use_count() > 0);
}



template <typename T, typename Allocator, typename Y, typename ... Args>
typename shared_ptr<T>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& alloc, Args&& ... args)
{
    static_assert(sizeof(T), "T must be complete type");
    typedef typename shared_ptr<T>::operation_error operation_error;

    typedef detail::type_stored_by_value<T, Allocator, operation_error> shared_count_t;
    typedef typename shared_count_t::allocator_type SharedCountAllocator;
    SharedCountAllocator sharedCountAlloc;

    shared_count_t* ptr = sharedCountAlloc.allocate(1);
    if (!ptr)
    {
        return operation_error(std::errc::not_enough_memory);
    }
    detail::allocation_scoped_guard<shared_count_t*, SharedCountAllocator> allocationGuard(sharedCountAlloc, ptr, 1);

    operation_error err = nestl::detail::construct<operation_error>(ptr, sharedCountAlloc, sharedCountAlloc);
    if (err)
    {
        return err;
    }

    err = ptr->initialize(std::forward<Args>(args) ...);
    if (err)
    {
        return err;
    }

    sp = shared_ptr<Y>(ptr->get(), ptr);

    allocationGuard.release();
    return operation_error();
}


template <typename T, typename Y, typename ... Args>
typename shared_ptr<T>::operation_error make_shared_ex(shared_ptr<Y>& sp, Args&& ... args)
{
    nestl::allocator<T> alloc;
    return make_shared_ex_a<T>(sp, alloc, std::forward<Args>(args) ...);
}

template <typename T, typename ... Args>
typename shared_ptr<T>::operation_error make_shared(shared_ptr<T>& sp, Args&& ... args)
{
    return make_shared_ex<T>(sp, std::forward<Args>(args) ...);
}


} // namespace nestl

#endif // NESTL_SHARED_PTR_HPP
