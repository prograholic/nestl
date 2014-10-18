#ifndef NESTL_VECTOR_HPP
#define NESTL_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/algorithm.hpp>
#include <nestl/memory.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/operation_error.hpp>
#include <nestl/initializer_list.hpp>
#include <nestl/assert.hpp>
#include <nestl/move.hpp>
#include <nestl/system_error.hpp>
#include <nestl/iterator.hpp>


namespace nestl
{

/**
 * @brief
 */
template <typename T, typename Allocator = nestl::allocator<T> >
class vector : private nestl::noncopyable
{
public:

    typedef T                                                               value_type;
    typedef Allocator                                                       allocator_type;
    typedef std::size_t                                                     size_type;
    typedef std::ptrdiff_t                                                  difference_type;
    typedef T&                                                              reference;
    typedef const T&                                                        const_reference;
    typedef typename nestl::allocator_traits<allocator_type>::pointer       pointer;
    typedef typename nestl::allocator_traits<allocator_type>::const_pointer const_pointer;

    typedef pointer                                                         iterator;
    typedef const_pointer                                                   const_iterator;
    typedef nestl::reverse_iterator<iterator>                               reverse_iterator;
    typedef nestl::reverse_iterator<const_iterator>                         const_reverse_iterator;

    typedef nestl::error_condition                                          operation_error;

    typedef nestl::result_with_operation_error<iterator, operation_error>   iterator_with_operation_error;


// constructors
    explicit vector(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    explicit vector(vector&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

// destructor
    ~vector() NESTL_NOEXCEPT_SPEC;

// allocator support
    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC;

// assignment operators and functions
#if NESTL_HAS_RVALUE_REF
    vector& operator=(vector&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    operation_error assign_copy(const vector& other) NESTL_NOEXCEPT_SPEC;

    operation_error assign(size_type n, const_reference val = value_type()) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    operation_error assign(InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_INITIALIZER_LIST
    operation_error assign(nestl::initializer_list<value_type> ilist) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_INITIALIZER_LIST */


// element access
    reference operator[](size_type pos) NESTL_NOEXCEPT_SPEC;

    const_reference operator[](size_type pos) const NESTL_NOEXCEPT_SPEC;

    reference front() NESTL_NOEXCEPT_SPEC;

    const_reference front() const NESTL_NOEXCEPT_SPEC;

    reference back() NESTL_NOEXCEPT_SPEC;

    const_reference back() const NESTL_NOEXCEPT_SPEC;

    pointer data() NESTL_NOEXCEPT_SPEC;

    const_pointer data() const NESTL_NOEXCEPT_SPEC;

// iterators
    iterator begin() NESTL_NOEXCEPT_SPEC;

    const_iterator begin() const NESTL_NOEXCEPT_SPEC;

    const_iterator cbegin() const NESTL_NOEXCEPT_SPEC;

    iterator end() NESTL_NOEXCEPT_SPEC;

    const_iterator end() const NESTL_NOEXCEPT_SPEC;

    const_iterator cend() const NESTL_NOEXCEPT_SPEC;

    reverse_iterator rbegin() NESTL_NOEXCEPT_SPEC;

    const_reverse_iterator rbegin() const NESTL_NOEXCEPT_SPEC;

    const_reverse_iterator crbegin() const NESTL_NOEXCEPT_SPEC;

    reverse_iterator rend() NESTL_NOEXCEPT_SPEC;

    const_reverse_iterator rend() const NESTL_NOEXCEPT_SPEC;

    const_reverse_iterator crend() const NESTL_NOEXCEPT_SPEC;

// capacity
    bool empty() const NESTL_NOEXCEPT_SPEC;

    size_type size() const NESTL_NOEXCEPT_SPEC;

    size_type max_size() const NESTL_NOEXCEPT_SPEC;

    operation_error reserve(size_type new_cap) NESTL_NOEXCEPT_SPEC;

    size_type capacity() const NESTL_NOEXCEPT_SPEC;

    operation_error shrink_to_fit() NESTL_NOEXCEPT_SPEC;

// modifiers
    void clear() NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert(const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    iterator_with_operation_error insert(const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    operation_error insert(const_iterator pos, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template<typename InputIterator>
    operation_error insert(const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_INITIALIZER_LIST
    iterator_with_operation_error insert(const_iterator pos, nestl::initializer_list<T> ilist) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_INITIALIZER_LIST */

#if NESTL_HAS_VARIADIC_TEMPLATES
    template<typename ... Args>
    iterator_with_operation_error emplace(const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC;
#else /* NESTL_HAS_VARIADIC_TEMPLATES */
    template<typename Arg>
    iterator_with_operation_error emplace(const_iterator pos, const Arg& arg) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_VARIADIC_TEMPLATES */

    iterator_with_operation_error erase(const_iterator pos) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error erase(const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    operation_error push_back(const value_type& value) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    operation_error push_back(value_type&& value) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */


#if NESTL_HAS_VARIADIC_TEMPLATES
    template<typename ... Args>
    operation_error emplace_back(Args&&... args) NESTL_NOEXCEPT_SPEC;
#else /* NESTL_HAS_VARIADIC_TEMPLATES */
    operation_error emplace_back() NESTL_NOEXCEPT_SPEC;

    template<typename Arg>
    operation_error emplace_back(const Arg& arg) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_VARIADIC_TEMPLATES */

    void pop_back() NESTL_NOEXCEPT_SPEC;

    operation_error resize(size_type count) NESTL_NOEXCEPT_SPEC;

    operation_error resize(size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    void swap(vector& other) NESTL_NOEXCEPT_SPEC;

private:
    allocator_type m_allocator;

    pointer m_start;
    pointer m_finish;
    pointer m_end_of_storage;

    void swap_data(vector& other) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    void move_assign(const nestl::true_type& /* true_val */, vector&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    template <typename InputIterator>
    operation_error assign_iterator(std::random_access_iterator_tag /* tag */,
                                    InputIterator first,
                                    InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    operation_error assign_iterator(std::input_iterator_tag /* tag */,
                                    InputIterator first,
                                    InputIterator last) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_VARIADIC_TEMPLATES
    template <typename ... Args>
    iterator_with_operation_error insert_value(const_iterator pos, Args&& ... args) NESTL_NOEXCEPT_SPEC;
#else /* NESTL_HAS_VARIADIC_TEMPLATES */
    template <typename Arg>
    iterator_with_operation_error insert_value(const_iterator pos, const Arg& arg) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_VARIADIC_TEMPLATES */

    template <typename InputIterator>
    iterator_with_operation_error insert_range(const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_VARIADIC_TEMPLATES
    template <typename ... Args>
    operation_error do_resize(size_type count, Args&& ... args) NESTL_NOEXCEPT_SPEC;
#else /* NESTL_HAS_VARIADIC_TEMPLATES */
    template <typename Arg>
    operation_error do_resize(size_type count, const Arg& arg) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_VARIADIC_TEMPLATES */

    operation_error grow(size_type requiredCapacity) NESTL_NOEXCEPT_SPEC;

    operation_error do_reserve(size_type new_cap) NESTL_NOEXCEPT_SPEC;
};

/**
 * @brief specialization of class_traits for nestl::vector
 *
 * Allows construct one vector from another (emulate copy construction)
 */
template <typename T, typename VectorAllocator>
struct class_traits <nestl::vector<T, VectorAllocator> >
{
    typedef nestl::vector<T, VectorAllocator> vector_t;

    template <typename OperationError, typename Allocator>
    static OperationError construct(vector_t* ptr, Allocator& alloc) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);

        return OperationError();
    }

#if NESTL_HAS_RVALUE_REF
    template <typename OperationError, typename Allocator>
    static OperationError construct(vector_t* ptr, Allocator& alloc, vector_t&& other) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr, nestl::move(other));

        return OperationError();
    }
#endif /* NESTL_HAS_RVALUE_REF */

    template <typename OperationError, typename Allocator>
    static OperationError construct(vector_t* ptr, Allocator& alloc, const vector_t& other) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);

        vector_t* end = ptr + 1;
        nestl::detail::destruction_scoped_guard<vector_t*, Allocator> guard(ptr, end, alloc);

        OperationError err = ptr->assign_copy(other);
        if (err)
        {
            return err;
        }

        guard.release();

        return err;
    }
};


template <typename T, typename Allocator>
bool operator == (const nestl::vector<T, Allocator>& left,
                  const nestl::vector<T, Allocator>& right) NESTL_NOEXCEPT_SPEC
{
    typedef nestl::vector<T, Allocator> vector_t;
    if (left.size() != right.size())
    {
        return false;
    }

    typename vector_t::const_iterator leftIt = left.cbegin();
    typename vector_t::const_iterator leftEnd = left.cend();

    typename vector_t::const_iterator rightIt = right.cbegin();


    while (leftIt != leftEnd)
    {
        if (!(*leftIt == *rightIt))
        {
            return false;
        }

        ++leftIt;
        ++rightIt;
    }

    return true;
}



/// Implementation

template <typename T, typename A>
vector<T, A>::vector(const allocator_type& alloc) NESTL_NOEXCEPT_SPEC
    : m_allocator(alloc)
    , m_start(0)
    , m_finish(0)
    , m_end_of_storage(0)
{
}


#if NESTL_HAS_RVALUE_REF
template <typename T, typename A>
vector<T, A>::vector(vector&& other) NESTL_NOEXCEPT_SPEC
    : m_allocator(nestl::move(other.get_allocator()))
    , m_start(0)
    , m_finish(0)
    , m_end_of_storage(0)
{
    this->swap_data(other);
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename A>
vector<T, A>::~vector() NESTL_NOEXCEPT_SPEC
{
    nestl::detail::destroy(m_allocator, m_start, m_finish);
    m_allocator.deallocate(m_start, m_end_of_storage - m_start);
}

template <typename T, typename A>
typename vector<T, A>::allocator_type
vector<T, A>::get_allocator() const NESTL_NOEXCEPT_SPEC
{
    return m_allocator;
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename A>
vector<T, A>&
vector<T, A>::operator=(vector&& other) NESTL_NOEXCEPT_SPEC
{
    move_assign(typename nestl::allocator_traits<allocator_type>::propagate_on_container_move_assignment(), nestl::move(other));
    return *this;
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::assign_copy(const vector& other) NESTL_NOEXCEPT_SPEC
{
    return this->assign(other.cbegin(), other.cend());
}

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::assign(size_type n, const_reference val) NESTL_NOEXCEPT_SPEC
{
    vector tmp; tmp.swap(*this);

    NESTL_ASSERT(empty());

    operation_error err = grow(n);
    if (err)
    {
        return err;
    }

    while (n > 0)
    {
        err = this->push_back(val);
        if (err)
        {
            return err;
        }
        --n;
    }

    return operation_error();
}

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::operation_error
vector<T, A>::assign(InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    vector tmp; tmp.swap(*this);

    return assign_iterator(typename std::iterator_traits<InputIterator>::iterator_category(), first, last);
}

#if NESTL_HAS_INITIALIZER_LIST
template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::assign(nestl::initializer_list<typename vector<T, A>::value_type> ilist) NESTL_NOEXCEPT_SPEC
{
    return this->assign(ilist.begin(), ilist.end());
}
#endif /* NESTL_HAS_INITIALIZER_LIST */

template <typename T, typename A>
typename vector<T, A>::reference
vector<T, A>::operator[](typename vector<T, A>::size_type pos) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(pos < size());
    return m_start[pos];
}

template <typename T, typename A>
typename vector<T, A>::const_reference
vector<T, A>::operator[](typename vector<T, A>::size_type pos) const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(pos < size());
    return m_start[pos];
}

template <typename T, typename A>
typename vector<T, A>::reference
vector<T, A>::front() NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(!empty());
    return *begin();
}

template <typename T, typename A>
typename vector<T, A>::const_reference
vector<T, A>::front() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(!empty());
    return *begin();
}

template <typename T, typename A>
typename vector<T, A>::reference
vector<T, A>::back() NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename vector<T, A>::const_reference
vector<T, A>::back() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename vector<T, A>::pointer
vector<T, A>::data() NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(!empty());
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_pointer
vector<T, A>::data() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(!empty());
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::iterator
vector<T, A>::begin() NESTL_NOEXCEPT_SPEC
{
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator
vector<T, A>::begin() const NESTL_NOEXCEPT_SPEC
{
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator
vector<T, A>::cbegin() const NESTL_NOEXCEPT_SPEC
{
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::iterator
vector<T, A>::end() NESTL_NOEXCEPT_SPEC
{
    return m_finish;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator
vector<T, A>::end() const NESTL_NOEXCEPT_SPEC
{
    return m_finish;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator
vector<T, A>::cend() const NESTL_NOEXCEPT_SPEC
{
    return m_finish;
}

template <typename T, typename A>
typename vector<T, A>::reverse_iterator
vector<T, A>::rbegin() NESTL_NOEXCEPT_SPEC
{
    return reverse_iterator(m_finish);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator
vector<T, A>::rbegin() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(m_finish);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator
vector<T, A>::crbegin() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(m_finish);
}

template <typename T, typename A>
typename vector<T, A>::reverse_iterator
vector<T, A>::rend() NESTL_NOEXCEPT_SPEC
{
    return reverse_iterator(m_start);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator
vector<T, A>::rend() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(m_start);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator
vector<T, A>::crend() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(m_start);
}

template <typename T, typename A>
bool vector<T, A>::empty() const NESTL_NOEXCEPT_SPEC
{
    return m_start == m_finish;
}

template <typename T, typename A>
typename vector<T, A>::size_type
vector<T, A>::size() const NESTL_NOEXCEPT_SPEC
{
    return nestl::distance(m_start, m_finish);
}

template <typename T, typename A>
typename vector<T, A>::size_type
vector<T, A>::max_size() const NESTL_NOEXCEPT_SPEC
{
    return nestl::allocator_traits<A>::max_size(m_allocator);
}

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::reserve(size_type new_cap) NESTL_NOEXCEPT_SPEC
{
    if (new_cap <= capacity())
    {
        return operation_error();
    }
    else
    {
        if (new_cap > max_size())
        {
            /// @todo add support for value_too_large
            return operation_error(nestl::errc::invalid_argument);
        }
    }

    return do_reserve(new_cap);
}

template <typename T, typename A>
typename vector<T, A>::size_type
vector<T, A>::capacity() const NESTL_NOEXCEPT_SPEC
{
    return nestl::distance(m_start, m_end_of_storage);
}

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::shrink_to_fit() NESTL_NOEXCEPT_SPEC
{
    if (capacity() > size())
    {
        return do_reserve(size());
    }

    return operation_error();
}


template <typename T, typename A>
void vector<T, A>::clear() NESTL_NOEXCEPT_SPEC
{
    nestl::detail::destroy(m_allocator, m_start, m_finish);
    m_finish = m_start;
}

template <typename T, typename A>
typename vector<T, A>::iterator_with_operation_error
vector<T, A>::insert(const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return insert_value(pos, value);
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename A>
typename vector<T, A>::iterator_with_operation_error
vector<T, A>::insert(const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC
{
    return insert_value(pos, nestl::move(value));
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename A>
template<typename InputIterator>
typename vector<T, A>::operation_error vector<T, A>::insert(const_iterator pos,
                                                            InputIterator first,
                                                            InputIterator last) NESTL_NOEXCEPT_SPEC
{
    return this->insert_range(pos, first, last).error();
}

#if NESTL_HAS_INITIALIZER_LIST
template <typename T, typename A>
typename vector<T, A>::iterator_with_operation_error
vector<T, A>::insert(const_iterator pos, nestl::initializer_list<T> ilist) NESTL_NOEXCEPT_SPEC
{
    return this->insert_range(pos, std::begin(ilist), std::end(ilist));
}
#endif /* NESTL_HAS_INITIALIZER_LIST */


template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::push_back(const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return this->insert(this->cend(), value).error();
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::push_back(value_type&& value) NESTL_NOEXCEPT_SPEC
{
    return this->insert(this->cend(), nestl::move(value)).error();
}
#endif /* NESTL_HAS_RVALUE_REF */

#if NESTL_HAS_VARIADIC_TEMPLATES

template <typename T, typename A>
template<typename ... Args>
typename vector<T, A>::operation_error
vector<T, A>::emplace_back(Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    return insert_value(this->cend(), std::forward<Args>(args) ...).error();
}

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

template <typename T, typename A>
template<typename Arg>
typename vector<T, A>::operation_error
vector<T, A>::emplace_back(const Arg& arg) NESTL_NOEXCEPT_SPEC
{
    return insert_value(this->cend(), arg).error();
}

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::resize(size_type count) NESTL_NOEXCEPT_SPEC
{
    return do_resize(count);
}

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::resize(size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return do_resize(count, value);
}

template <typename T, typename A>
void vector<T, A>::swap(vector& other) NESTL_NOEXCEPT_SPEC
{
    std::swap(m_allocator, other.m_allocator);
    this->swap_data(other);
}

/// Private implementation

template <typename T, typename A>
void vector<T, A>::swap_data(vector& other) NESTL_NOEXCEPT_SPEC
{
    std::swap(this->m_start, other.m_start);
    std::swap(this->m_finish, other.m_finish);
    std::swap(this->m_end_of_storage, other.m_end_of_storage);
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename A>
void vector<T, A>::move_assign(const nestl::true_type& /* true_val */, vector&& other) NESTL_NOEXCEPT_SPEC
{
    const vector tmp(nestl::move(*this));
    this->swap_data(other);
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::operation_error
vector<T, A>::assign_iterator(std::random_access_iterator_tag /* tag */,
                              InputIterator first,
                              InputIterator last) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(empty());
    size_t required_size = nestl::distance(first, last);

    operation_error err = this->grow(required_size);
    if (err)
    {
        return err;
    }

    return this->assign_iterator(std::input_iterator_tag(), first, last);
}

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::operation_error
vector<T, A>::assign_iterator(std::input_iterator_tag /* tag */,
                              InputIterator first,
                              InputIterator last) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(empty());

    while (first != last)
    {
        operation_error err = this->push_back(*first);
        if (err)
        {
            return err;
        }

        ++first;
    }

    return operation_error();
}

#if NESTL_HAS_VARIADIC_TEMPLATES

template <typename T, typename A>
template <typename ... Args>
typename vector<T, A>::iterator_with_operation_error
vector<T, A>::insert_value(const_iterator pos, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(pos >= m_start);
    NESTL_ASSERT(pos <= m_finish);

    /// we should calculate offset before possible reallocation
    size_t offset = pos - m_start;

    if (capacity() == size())
    {
        operation_error err = this->grow(capacity() + 1);
        if (err)
        {
            return nestl::make_result_with_operation_error(this->begin(), err);
        }
    }
    NESTL_ASSERT(capacity() > size());

    value_type* last = m_finish;
    value_type* first = m_start + offset;
    for (value_type* val = last; val != first; --val)
    {
        value_type* oldLocation = val - 1;
        operation_error err = nestl::detail::construct<operation_error>(val, m_allocator, *oldLocation);
        if (err)
        {
            return nestl::make_result_with_operation_error(this->begin(), err);
        }
        nestl::detail::destroy(m_allocator, oldLocation, val);
    }


    operation_error err = nestl::detail::construct<operation_error>(first, m_allocator, std::forward<Args>(args) ...);
    if (err)
    {
        return nestl::make_result_with_operation_error(this->begin(), err);
    }

    ++m_finish;
    return nestl::make_result_with_operation_error(first, err);
}

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

template <typename T, typename A>
template <typename Arg>
typename vector<T, A>::iterator_with_operation_error
vector<T, A>::insert_value(const_iterator pos, const Arg& arg) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(pos >= m_start);
    NESTL_ASSERT(pos <= m_finish);

    /// we should calculate offset before possible reallocation
    size_t offset = pos - m_start;

    if (capacity() == size())
    {
        operation_error err = this->grow(capacity() + 1);
        if (err)
        {
            return nestl::make_result_with_operation_error(this->begin(), err);
        }
    }
    NESTL_ASSERT(capacity() > size());

    value_type* last = m_finish;
    value_type* first = m_start + offset;
    for (value_type* val = last; val != first; --val)
    {
        value_type* oldLocation = val - 1;
        operation_error err = nestl::detail::construct<operation_error>(val, m_allocator, *oldLocation);
        if (err)
        {
            return nestl::make_result_with_operation_error(this->begin(), err);
        }
        nestl::detail::destroy(m_allocator, oldLocation, val);
    }


    operation_error err = nestl::detail::construct<operation_error>(first, m_allocator, arg);
    if (err)
    {
        return nestl::make_result_with_operation_error(this->begin(), err);
    }

    ++m_finish;
    return nestl::make_result_with_operation_error(first, err);
}

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */


template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::iterator_with_operation_error
vector<T, A>::insert_range(const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(pos >= m_start);
    NESTL_ASSERT(pos <= m_finish);

    /// we should calculate offset before possible reallocation
    size_t offset = pos - m_start;

    iterator_with_operation_error err;
    for ( ; first != last; ++first, ++offset)
    {
        const_iterator newPos = m_start + offset;
        err = insert_value(newPos, *first);
        if (err)
        {
            return err;
        }
    }

    return err;
}


#if NESTL_HAS_VARIADIC_TEMPLATES

template <typename T, typename A>
template <typename ... Args>
typename vector<T, A>::operation_error
vector<T, A>::do_resize(size_type count, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    if (count <= size())
    {
        nestl::detail::destroy(m_allocator, m_start + count, m_finish);
        m_finish = m_start + count;
    }
    else
    {
        operation_error err = grow(count);
        if (err)
        {
            return err;
        }

        while (m_finish < m_start + count)
        {
            operation_error err = nestl::detail::construct<operation_error>(m_finish, m_allocator, std::forward<Args>(args) ...);
            if (err)
            {
                return err;
            }
            ++m_finish;
        }
    }

    return operation_error();
}

#else /* NESTL_HAS_VARIADIC_TEMPLATES */

template <typename T, typename A>
template <typename Arg>
typename vector<T, A>::operation_error
vector<T, A>::do_resize(size_type count, const Arg& arg) NESTL_NOEXCEPT_SPEC
{
    if (count <= size())
    {
        nestl::detail::destroy(m_allocator, m_start + count, m_finish);
        m_finish = m_start + count;
    }
    else
    {
        operation_error err = grow(count);
        if (err)
        {
            return err;
        }

        while (m_finish < m_start + count)
        {
            operation_error err = nestl::detail::construct<operation_error>(m_finish, m_allocator, arg);
            if (err)
            {
                return err;
            }
            ++m_finish;
        }
    }

    return operation_error();
}

#endif /* NESTL_HAS_VARIADIC_TEMPLATES */


template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::grow(size_type requiredCapacity) NESTL_NOEXCEPT_SPEC
{
    size_t newCapacity = (((capacity() + 1) * 3) / 2);
    if (newCapacity < requiredCapacity)
    {
        newCapacity = requiredCapacity;
    }

    /// @bug overflow error
    NESTL_ASSERT(newCapacity > capacity());

    return reserve(newCapacity);
}

template <typename T, typename A>
typename vector<T, A>::operation_error
vector<T, A>::do_reserve(size_type new_cap) NESTL_NOEXCEPT_SPEC
{
    value_type* ptr = m_allocator.allocate(new_cap);
    if (!ptr)
    {
        return operation_error(nestl::errc::not_enough_memory);
    }
    nestl::detail::allocation_scoped_guard<value_type*, allocator_type> guard(m_allocator, ptr, new_cap);

    operation_error err = nestl::uninitialised_copy<operation_error>(m_start, m_finish, ptr, m_allocator);
    if (err)
    {
        return err;
    }

    const size_t current_size = size();
    nestl::detail::destroy(m_allocator, m_start, m_finish);
    m_allocator.deallocate(m_start, m_end_of_storage - m_start);

    m_start = ptr;
    m_finish = ptr + current_size;
    m_end_of_storage = ptr + new_cap;

    guard.release();
    return operation_error();
}

} // namespace nestl

#endif /* NESTL_VECTOR_HPP */
