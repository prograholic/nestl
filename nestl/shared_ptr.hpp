#ifndef NESTL_SHARED_PTR_HPP
#define NESTL_SHARED_PTR_HPP

#include <nestl/config.hpp>

#include <nestl/system_error.hpp>
#include <nestl/memory.hpp>
#include <nestl/allocator.hpp>
#include <nestl/alignment.hpp>
#include <nestl/assert.hpp>
#include <nestl/swap.hpp>
#include <nestl/move.hpp>

#include <nestl/detail/construct.hpp>


namespace nestl
{

namespace detail
{

class shared_count_base : private nestl::noncopyable
{
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
        NESTL_ASSERT(m_use_count >= 0);
        return m_use_count;
    }

    void add_ref() NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(m_use_count >= 0);
        ++m_use_count;
    }

    void release() NESTL_NOEXCEPT_SPEC
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

    void weak_add_ref() NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(m_weak_use_count >= 0);
        ++m_weak_use_count;
    }

    void weak_release() NESTL_NOEXCEPT_SPEC
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

    virtual void destroy_value() NESTL_NOEXCEPT_SPEC = 0;

    virtual void destroy_self() NESTL_NOEXCEPT_SPEC = 0;
};


template <typename T, typename TypeAllocator, typename OperationError>
class type_stored_by_value : public shared_count_base
{
public:

    typedef OperationError operation_error;

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
        nestl::detail::destroy(m_allocator, value, value + 1);
    }

    virtual void destroy_self() NESTL_NOEXCEPT_SPEC
    {
        allocator_type alloc(m_allocator);

        alloc.destroy(this);
        alloc.deallocate(this, 1);
    }

#if defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES)

    template <typename ... Args>
    operation_error initialize(Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
        operation_error err = nestl::detail::construct<operation_error>(get(), m_allocator, nestl::forward<Args>(args) ...);
        return err;
    }

#else /* defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES) */

    operation_error initialize() NESTL_NOEXCEPT_SPEC
    {
        operation_error err = nestl::detail::construct<operation_error>(get(), m_allocator);
        return err;
    }

    template <typename Arg>
    operation_error initialize(const Arg& arg) NESTL_NOEXCEPT_SPEC
    {
        operation_error err = nestl::detail::construct<operation_error>(get(), m_allocator, arg);
        return err;
    }

#endif /* defined(NESTL_CONFIG_HAS_VARIADIC_TEMPLATES) */

private:
    nestl::aligned_buffer<T> m_value;
    allocator_type m_allocator;
};

} // namespace detail

template <typename T>
class weak_ptr;

template <typename T>
class shared_ptr
{
public:

    typedef T                      element_type;
    typedef T*                     pointer_type;
    typedef nestl::error_condition operation_error;

    /// constructors
    shared_ptr() NESTL_NOEXCEPT_SPEC;

    shared_ptr(const shared_ptr& other) NESTL_NOEXCEPT_SPEC;

    template <typename Y>
    shared_ptr(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    shared_ptr(shared_ptr&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    /// destructor
    ~shared_ptr() NESTL_NOEXCEPT_SPEC;

    /// assignment operators
    shared_ptr& operator=(const shared_ptr& other) NESTL_NOEXCEPT_SPEC;

    template <typename Y>
    shared_ptr& operator=(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    template <typename Y>
    shared_ptr& operator=(shared_ptr<Y>&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    /// assign_copy
    template <typename Y>
    operation_error assign_copy(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC;

    void reset() NESTL_NOEXCEPT_SPEC;

    void swap(shared_ptr& other) NESTL_NOEXCEPT_SPEC;

    /// observers
    element_type* get() const NESTL_NOEXCEPT_SPEC;

    element_type& operator*() const NESTL_NOEXCEPT_SPEC;

    element_type* operator->() const NESTL_NOEXCEPT_SPEC;

    long use_count() const NESTL_NOEXCEPT_SPEC;

    bool unique() const NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_EXPLICIT_OPERATOR
    explicit operator bool() const NESTL_NOEXCEPT_SPEC
    {
        return (use_count() > 0);
    }
#else /* NESTL_HAS_EXPLICIT_OPERATOR */
    typedef element_type* shared_ptr::*unspecified_bool_type;
    operator unspecified_bool_type() const NESTL_NOEXCEPT_SPEC
    {
        return use_count() == 0 ? 0 : &shared_ptr::m_ptr;
    }
#endif /* NESTL_HAS_EXPLICIT_OPERATOR */

private:
    element_type* m_ptr;
    detail::shared_count_base* m_refcount;

    shared_ptr(element_type* ptr, detail::shared_count_base* refcount) NESTL_NOEXCEPT_SPEC
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

#if NESTL_HAS_VARIADIC_TEMPLATES
    template <typename Type, typename Allocator, typename Y, typename ... Args>
    friend
    typename shared_ptr<Type>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& alloc, Args&& ... args);
#else /* NESTL_HAS_VARIADIC_TEMPLATES */
    template <typename Type, typename Allocator, typename Y>
    friend
    typename shared_ptr<Type>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& alloc);

    template <typename Type, typename Allocator, typename Y, typename Arg>
    friend
    typename shared_ptr<Type>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& alloc, const Arg& arg);
#endif /* NESTL_HAS_VARIADIC_TEMPLATES */
};


template <typename T>
class weak_ptr
{
public:
    typedef T                      element_type;
    typedef T*                     pointer_type;
    typedef nestl::error_condition operation_error;

    weak_ptr() NESTL_NOEXCEPT_SPEC
        : m_ptr(0)
        , m_refcount(0)
    {
    }

#if NESTL_HAS_RVALUE_REF
    weak_ptr(weak_ptr&& other) NESTL_NOEXCEPT_SPEC
        : m_ptr(other.m_ptr)
        , m_refcount(other.m_refcount)
    {
        other.m_ptr = 0;
        other.m_refcount = 0;
    }
#endif /* NESTL_HAS_RVALUE_REF */

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

#if NESTL_HAS_RVALUE_REF
    weak_ptr& operator=(weak_ptr&& other) NESTL_NOEXCEPT_SPEC
    {
        const weak_ptr tmp(nestl::move(*this));

        m_ptr = other.m_ptr;
        m_refcount = other.m_refcount;

        return *this;
    }
#endif /* NESTL_HAS_RVALUE_REF */

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
    detail::shared_count_base* m_refcount;

};




/// Implementation

template <typename T>
shared_ptr<T>::shared_ptr() NESTL_NOEXCEPT_SPEC
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

#if NESTL_HAS_RVALUE_REF
template <typename T>
shared_ptr<T>::shared_ptr(shared_ptr&& other) NESTL_NOEXCEPT_SPEC
    : m_ptr(0)
    , m_refcount(0)
{
    this->swap(other);
}
#endif /* NESTL_HAS_RVALUE_REF */

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

#if NESTL_HAS_RVALUE_REF
template <typename T>
template <typename Y>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<Y>&& other) NESTL_NOEXCEPT_SPEC
{
    if (&other != this)
    {
        const shared_ptr tmp(nestl::move(*this));

        this->swap(other);
    }

    return *this;
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T>
template <typename Y>
typename shared_ptr<T>::operation_error shared_ptr<T>::assign_copy(const shared_ptr<Y>& other) NESTL_NOEXCEPT_SPEC
{
    *this = other;
    return operation_error();
}


template <typename T>
void shared_ptr<T>::reset() NESTL_NOEXCEPT_SPEC
{
    shared_ptr tmp; tmp.swap(*this);
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr& other) NESTL_NOEXCEPT_SPEC
{
    nestl::swap(m_ptr, other.m_ptr);
    nestl::swap(m_refcount, other.m_refcount);
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::get() const NESTL_NOEXCEPT_SPEC
{
    return m_ptr;
}

template <typename T>
typename shared_ptr<T>::element_type& shared_ptr<T>::operator*() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(m_ptr);
    return *m_ptr;
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::operator->() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(m_ptr);
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

#if NESTL_HAS_VARIADIC_TEMPLATES
template <typename T, typename Allocator, typename Y, typename ... Args>
typename shared_ptr<T>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& /* alloc */, Args&& ... args)
{
    NESTL_STATIC_ASSERT(sizeof(T), "T must be complete type");
    typedef typename shared_ptr<T>::operation_error operation_error;

    typedef detail::type_stored_by_value<T, Allocator, operation_error> shared_count_t;
    typedef typename shared_count_t::allocator_type SharedCountAllocator;
    SharedCountAllocator sharedCountAlloc;

    shared_count_t* ptr = sharedCountAlloc.allocate(1);
    if (!ptr)
    {
        return operation_error(nestl::errc::not_enough_memory);
    }
    detail::allocation_scoped_guard<shared_count_t*, SharedCountAllocator> allocationGuard(sharedCountAlloc, ptr, 1);

    operation_error err = nestl::detail::construct<operation_error>(ptr, sharedCountAlloc, sharedCountAlloc);
    if (err)
    {
        return err;
    }

    err = ptr->initialize(nestl::forward<Args>(args) ...);
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
    return make_shared_ex_a<T>(sp, alloc, nestl::forward<Args>(args) ...);
}

template <typename T, typename ... Args>
typename shared_ptr<T>::operation_error make_shared(shared_ptr<T>& sp, Args&& ... args)
{
    return make_shared_ex<T>(sp, nestl::forward<Args>(args) ...);
}

#else /* NESTL_HAS_VARIADIC_TEMPLATES */


template <typename T, typename Allocator, typename Y>
typename shared_ptr<T>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& /* alloc */)
{
    NESTL_STATIC_ASSERT(sizeof(T), "T must be complete type");
    typedef typename shared_ptr<T>::operation_error operation_error;

    typedef detail::type_stored_by_value<T, Allocator, operation_error> shared_count_t;
    typedef typename shared_count_t::allocator_type SharedCountAllocator;
    SharedCountAllocator sharedCountAlloc;

    shared_count_t* ptr = sharedCountAlloc.allocate(1);
    if (!ptr)
    {
        return operation_error(nestl::errc::not_enough_memory);
    }
    detail::allocation_scoped_guard<shared_count_t*, SharedCountAllocator> allocationGuard(sharedCountAlloc, ptr, 1);

    operation_error err = nestl::detail::construct<operation_error>(ptr, sharedCountAlloc, sharedCountAlloc);
    if (err)
    {
        return err;
    }

    err = ptr->initialize();
    if (err)
    {
        return err;
    }

    sp = shared_ptr<Y>(ptr->get(), ptr);

    allocationGuard.release();
    return operation_error();
}

template <typename T, typename Allocator, typename Y, typename Arg>
typename shared_ptr<T>::operation_error make_shared_ex_a(shared_ptr<Y>& sp, Allocator& /* alloc */, const Arg& arg)
{
    NESTL_STATIC_ASSERT(sizeof(T), "T must be complete type");
    typedef typename shared_ptr<T>::operation_error operation_error;

    typedef detail::type_stored_by_value<T, Allocator, operation_error> shared_count_t;
    typedef typename shared_count_t::allocator_type SharedCountAllocator;
    SharedCountAllocator sharedCountAlloc;

    shared_count_t* ptr = sharedCountAlloc.allocate(1);
    if (!ptr)
    {
        return operation_error(nestl::errc::not_enough_memory);
    }
    detail::allocation_scoped_guard<shared_count_t*, SharedCountAllocator> allocationGuard(sharedCountAlloc, ptr, 1);

    operation_error err = nestl::detail::construct<operation_error>(ptr, sharedCountAlloc, sharedCountAlloc);
    if (err)
    {
        return err;
    }

    err = ptr->initialize(arg);
    if (err)
    {
        return err;
    }

    sp = shared_ptr<Y>(ptr->get(), ptr);

    allocationGuard.release();
    return operation_error();
}




template <typename T, typename Y>
typename shared_ptr<T>::operation_error make_shared_ex(shared_ptr<Y>& sp)
{
    nestl::allocator<T> alloc;
    return make_shared_ex_a<T>(sp, alloc);
}

template <typename T, typename Y, typename Arg>
typename shared_ptr<T>::operation_error make_shared_ex(shared_ptr<Y>& sp, const Arg& arg)
{
    nestl::allocator<T> alloc;
    return make_shared_ex_a<T>(sp, alloc, arg);
}




template <typename T>
typename shared_ptr<T>::operation_error make_shared(shared_ptr<T>& sp)
{
    return make_shared_ex<T>(sp);
}

template <typename T, typename Arg>
typename shared_ptr<T>::operation_error make_shared(shared_ptr<T>& sp, const Arg& arg)
{
    return make_shared_ex<T>(sp, arg);
}

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */


} // namespace nestl

#endif // NESTL_SHARED_PTR_HPP
