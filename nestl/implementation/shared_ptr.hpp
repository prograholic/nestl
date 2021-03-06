#ifndef NESTL_IMPLEMENTATION_SHARED_PTR_HPP
#define NESTL_IMPLEMENTATION_SHARED_PTR_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/alignment.hpp>
#include <nestl/class_operations.hpp>

#include <nestl/detail/destroy.hpp>

#include <cassert>

namespace nestl
{
namespace impl
{

class shared_count_base
{
    shared_count_base(const shared_count_base& ) = delete;
    shared_count_base& operator=(const shared_count_base& ) = delete;
public:

    shared_count_base() NESTL_NOEXCEPT_SPEC
        : m_use_count(0)
        , m_weak_use_count(0)
    {
    }

    virtual ~shared_count_base()
    {
    }

    long use_count() const NESTL_NOEXCEPT_SPEC
    {
		assert(m_use_count >= 0);
        return m_use_count;
    }

    void add_ref() NESTL_NOEXCEPT_SPEC
    {
		assert(m_use_count >= 0);
        ++m_use_count;
    }

    void release() NESTL_NOEXCEPT_SPEC
    {
		assert(m_use_count > 0);
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

    void weak_add_ref() NESTL_NOEXCEPT_SPEC
    {
		assert(m_weak_use_count >= 0);
        ++m_weak_use_count;
    }

    void weak_release() NESTL_NOEXCEPT_SPEC
    {
		assert(m_weak_use_count > 0);
        --m_weak_use_count;
        if ((m_weak_use_count == 0) && (m_use_count == 0))
        {
            destroy_self();
        }
    }

private:
    long m_use_count;
    long m_weak_use_count;

    virtual void destroy_value() NESTL_NOEXCEPT_SPEC = 0;

    virtual void destroy_self() NESTL_NOEXCEPT_SPEC = 0;
};


template <typename T, typename TypeAllocator>
class type_stored_by_value : public shared_count_base
{
public:

    typedef typename TypeAllocator:: template rebind<type_stored_by_value>::other allocator_type;

    type_stored_by_value(const allocator_type& alloc) NESTL_NOEXCEPT_SPEC
        : shared_count_base()
        , m_value()
        , m_allocator(alloc)
    {
    }

    T* get()
    {
        return reinterpret_cast<T*>(&m_value);
    }

    virtual void destroy_value() NESTL_NOEXCEPT_SPEC
    {
        T* value = get();
        nestl::detail::destroy(value, value + 1);
    }

    virtual void destroy_self() NESTL_NOEXCEPT_SPEC
    {
        allocator_type alloc(m_allocator);

        nestl::detail::destroy(this);
        alloc.deallocate(this, 1);
    }

    template <typename OperationError, typename ... Args>
    void initialize(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        nestl::class_operations::construct(err, get(), std::forward<Args>(args) ...);
    }

private:
    nestl::aligned_buffer<T> m_value;
    allocator_type m_allocator;
};


template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr
{
public:

    typedef T                      element_type;
    typedef T*                     pointer_type;

    /// constructors
    shared_ptr() NESTL_NOEXCEPT_SPEC;

    shared_ptr(std::nullptr_t) NESTL_NOEXCEPT_SPEC;

    shared_ptr(const shared_ptr& other) NESTL_NOEXCEPT_SPEC;

    template <typename Y>
    shared_ptr(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC;

    shared_ptr(shared_ptr&& other) NESTL_NOEXCEPT_SPEC;

    /// destructor
    ~shared_ptr() NESTL_NOEXCEPT_SPEC;

    /// assignment operators
    shared_ptr& operator=(const shared_ptr& other) NESTL_NOEXCEPT_SPEC;

    template <typename Y>
    shared_ptr& operator=(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC;

    template <typename Y>
    shared_ptr& operator=(shared_ptr<Y>&& other) NESTL_NOEXCEPT_SPEC;

    void reset() NESTL_NOEXCEPT_SPEC;

    void swap(shared_ptr& other) NESTL_NOEXCEPT_SPEC;

    /// observers
    element_type* get() const NESTL_NOEXCEPT_SPEC;

    element_type& operator*() const NESTL_NOEXCEPT_SPEC;

    element_type* operator->() const NESTL_NOEXCEPT_SPEC;

    long use_count() const NESTL_NOEXCEPT_SPEC;

    bool unique() const NESTL_NOEXCEPT_SPEC;

    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return (use_count() > 0);
    }

private:
    element_type* m_ptr;
    shared_count_base* m_refcount;

    shared_ptr(element_type* ptr, shared_count_base* refcount) NESTL_NOEXCEPT_SPEC
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

    template <typename Y>
    friend class shared_ptr;

    template <typename Type, typename OperationError, typename Allocator, typename ... Args>
    friend
    shared_ptr<Type> make_shared_a_nothrow(OperationError& err, Allocator& alloc, Args&& ... args);
};


template <typename T>
class weak_ptr
{
public:
    typedef T                      element_type;
    typedef T*                     pointer_type;

    weak_ptr() NESTL_NOEXCEPT_SPEC
        : m_ptr(0)
        , m_refcount(0)
    {
    }

    weak_ptr(weak_ptr&& other) NESTL_NOEXCEPT_SPEC
        : m_ptr(other.m_ptr)
        , m_refcount(other.m_refcount)
    {
        other.m_ptr = 0;
        other.m_refcount = 0;
    }

    weak_ptr(const shared_ptr<T>& sp) NESTL_NOEXCEPT_SPEC
        : m_ptr(sp.m_ptr)
        , m_refcount(sp.m_refcount)
    {
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }
    }

    weak_ptr(const weak_ptr& other) NESTL_NOEXCEPT_SPEC
        : m_ptr(other.m_ptr)
        , m_refcount(other.m_refcount)
    {
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }
    }


    void reset() NESTL_NOEXCEPT_SPEC
    {
        if (m_refcount)
        {
            m_refcount->weak_release();
            m_refcount = 0;
        }

        m_ptr = 0;
    }

    ~weak_ptr() NESTL_NOEXCEPT_SPEC
    {
        if (m_refcount)
        {
            m_refcount->weak_release();
        }
    }

    weak_ptr& operator=(const shared_ptr<T>& other) NESTL_NOEXCEPT_SPEC
    {
        if (m_refcount)
        {
            m_refcount->weak_release();
        }

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }

        return *this;
    }

    weak_ptr& operator=(const weak_ptr& other) NESTL_NOEXCEPT_SPEC
    {
        if (m_refcount)
        {
            m_refcount->weak_release();
        }

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;
        if (m_refcount)
        {
            m_refcount->weak_add_ref();
        }

        return *this;
    }

    weak_ptr& operator=(weak_ptr&& other) NESTL_NOEXCEPT_SPEC
    {
        const weak_ptr tmp(std::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;

        return *this;
    }

    bool expired() const NESTL_NOEXCEPT_SPEC
    {
        return use_count() == 0;
    }

    long use_count() const NESTL_NOEXCEPT_SPEC
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
    shared_count_base* m_refcount;
};




/// Implementation

template <typename T>
shared_ptr<T>::shared_ptr() NESTL_NOEXCEPT_SPEC
    : m_ptr(0)
    , m_refcount(0)
{
}

template <typename T>
shared_ptr<T>::shared_ptr(std::nullptr_t) NESTL_NOEXCEPT_SPEC
    : m_ptr(0)
    , m_refcount(0)
{
}

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& other) NESTL_NOEXCEPT_SPEC
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
shared_ptr<T>::shared_ptr(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC
    : m_ptr(other.m_ptr)
    , m_refcount(other.m_refcount)
{
    if (m_refcount)
    {
        m_refcount->add_ref();
    }
}

template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other) NESTL_NOEXCEPT_SPEC
    : m_ptr(0)
    , m_refcount(0)
{
    this->swap(other);
}

template <typename T>
shared_ptr<T>::~shared_ptr() NESTL_NOEXCEPT_SPEC
{
    if (m_refcount)
    {
        m_refcount->release();
    }
}


template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& other) NESTL_NOEXCEPT_SPEC
{
    if (&other != this)
    {
        shared_ptr tmp; tmp.swap(*this);

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
shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC
{
    shared_ptr tmp;
    tmp.swap(*this);

    m_ptr = other.m_ptr;
    m_refcount = other.m_refcount;

    if (m_refcount)
    {
        m_refcount->add_ref();
    }

    return *this;
}

template <typename T>
template <typename Y>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<Y>&& other) NESTL_NOEXCEPT_SPEC
{
    {
        const shared_ptr tmp(std::move(*this));
    }
    
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;

    m_refcount = other.m_refcount;
    other.m_refcount = nullptr;

    return *this;
}

template <typename T>
void shared_ptr<T>::reset() NESTL_NOEXCEPT_SPEC
{
    shared_ptr tmp;
    tmp.swap(*this);
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr& other) NESTL_NOEXCEPT_SPEC
{
	std::swap(m_ptr, other.m_ptr);
	std::swap(m_refcount, other.m_refcount);
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::get() const NESTL_NOEXCEPT_SPEC
{
    return m_ptr;
}

template <typename T>
typename shared_ptr<T>::element_type& shared_ptr<T>::operator*() const NESTL_NOEXCEPT_SPEC
{
    assert(m_ptr);
    return *m_ptr;
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::operator->() const NESTL_NOEXCEPT_SPEC
{
    assert(m_ptr);
    return m_ptr;
}

template <typename T>
long shared_ptr<T>::use_count() const NESTL_NOEXCEPT_SPEC
{
    return m_refcount ? m_refcount->use_count() : 0;
}

template <typename T>
bool shared_ptr<T>::unique() const NESTL_NOEXCEPT_SPEC
{
    return (use_count() == 1);
}

template <typename T, typename OperationError, typename Allocator, typename ... Args>
shared_ptr<T> make_shared_a_nothrow(OperationError& err, Allocator& /* alloc */, Args&& ... args)
{
    static_assert(sizeof(T), "T must be complete type");
    typedef type_stored_by_value<T, Allocator> shared_count_t;
    typedef typename shared_count_t::allocator_type SharedCountAllocator;
    SharedCountAllocator sharedCountAlloc;

    shared_count_t* ptr = allocator_traits<SharedCountAllocator>::allocate(err, sharedCountAlloc, 1);
    if (err)
    {
        return {nullptr};
    }
    nestl::detail::deallocation_scoped_guard<shared_count_t*, SharedCountAllocator> deallocationGuard(sharedCountAlloc, ptr, 1);

    nestl::class_operations::construct(err, ptr, sharedCountAlloc);
    if (err)
    {
        return {nullptr};
    }

    ptr->initialize(err, std::forward<Args>(args) ...);
    if (err)
    {
        return {nullptr};
    }

    deallocationGuard.release();
    return shared_ptr<T>(ptr->get(), ptr);
}

template <typename T, typename OperationError, typename ... Args>
shared_ptr<T> make_shared_nothrow(OperationError& err, Args&& ... args)
{
    nestl::allocator<T> alloc;
	return make_shared_a_nothrow<T>(err, alloc, std::forward<Args>(args) ...);
}

} // namespace impl
} // namespace nestl

#endif /* NESTL_IMPLEMENTATION_SHARED_PTR_HPP */
