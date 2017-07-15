#ifndef NESTL_IMPLEMENTATION_VECTOR_HPP
#define NESTL_IMPLEMENTATION_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/algorithm.hpp>
#include <nestl/class_operations.hpp>

#include <nestl/detail/destroy.hpp>
#include <nestl/detail/uninitialised_copy.hpp>

#include <cassert>

namespace nestl
{
namespace impl
{

/**
 * @brief
 */
template <typename T, typename Allocator = nestl::allocator<T> >
class vector
{
    vector(const vector&) = delete;
    vector& operator=(const vector&) = delete;

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
    typedef std::reverse_iterator<iterator>                                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                           const_reverse_iterator;

// constructors
    explicit vector(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC;

    explicit vector(vector&& other) NESTL_NOEXCEPT_SPEC;

// destructor
    ~vector() NESTL_NOEXCEPT_SPEC;

// allocator support
    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC;

// assignment operators and functions
    vector& operator=(vector&& other) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void copy_nothrow(OperationError& err, const vector& other) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void assign_nothrow(OperationError& err, size_type n, const_reference val = value_type()) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename InputIterator>
    void assign_nothrow(OperationError& err, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;


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

    template <typename OperationError>
    void reserve_nothrow(OperationError& err, size_type new_cap) NESTL_NOEXCEPT_SPEC;

    size_type capacity() const NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void shrink_to_fit_nothrow(OperationError& err) NESTL_NOEXCEPT_SPEC;

// modifiers
    void clear() NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    iterator insert_nothrow(OperationError& err, const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    iterator insert_nothrow(OperationError& err, const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void insert_nothrow(OperationError& err, const_iterator pos, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template<typename OperationError, typename InputIterator>
    void insert_nothrow(OperationError& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    template<typename OperationError, typename ... Args>
    iterator emplace_nothrow(OperationError& err, const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    iterator erase_nothrow(OperationError& err, const_iterator pos) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    iterator erase_nothrow(OperationError& err, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void push_back_nothrow(OperationError& err, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void push_back_nothrow(OperationError& err, value_type&& value) NESTL_NOEXCEPT_SPEC;

    template<typename OperationError, typename ... Args>
    void emplace_back_nothrow(OperationError& err, Args&&... args) NESTL_NOEXCEPT_SPEC;

    void pop_back() NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void resize_nothrow(OperationError& err, size_type count) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void resize_nothrow(OperationError& err, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    void swap(vector& other) NESTL_NOEXCEPT_SPEC;

private:
    allocator_type m_allocator;

    pointer m_start;
    pointer m_finish;
    pointer m_end_of_storage;

    void swap_data(vector& other) NESTL_NOEXCEPT_SPEC;

    void move_assign(const std::true_type& /* true_val */, vector&& other) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename InputIterator>
    void assign_iterator(OperationError& err,
                         std::random_access_iterator_tag /* tag */,
                         InputIterator first,
                         InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename InputIterator>
    void assign_iterator(OperationError& err,
                         std::input_iterator_tag /* tag */,
                         InputIterator first,
                         InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename ... Args>
    iterator insert_value(OperationError& err, const_iterator pos, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename InputIterator>
    iterator insert_range(OperationError& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename ... Args>
    void do_resize(OperationError& err, size_type count, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void grow(OperationError& err, size_type requiredCapacity) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void do_reserve(OperationError& err, size_type new_cap) NESTL_NOEXCEPT_SPEC;
};

} // namespace impl

template <typename T, typename Allocator>
struct two_phase_initializator<nestl::impl::vector<T, Allocator>>
{
    typedef nestl::impl::vector<T, Allocator> vector_t;

    template <typename OperationError>
    static void construct(OperationError& err, vector_t& defaultConstructed, const vector_t& other) NESTL_NOEXCEPT_SPEC
    {
        defaultConstructed.copy_nothrow(err, other);
    }
};

namespace impl
{

template <typename T, typename Allocator>
bool operator == (const vector<T, Allocator>& left,
                  const vector<T, Allocator>& right) NESTL_NOEXCEPT_SPEC
{
    if (left.size() != right.size())
    {
        return false;
    }

    auto leftIt = left.cbegin();
    auto leftEnd = left.cend();

    auto rightIt = right.cbegin();


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


template <typename T, typename A>
vector<T, A>::vector(vector&& other) NESTL_NOEXCEPT_SPEC
	: m_allocator(std::move(other.get_allocator()))
    , m_start(0)
    , m_finish(0)
    , m_end_of_storage(0)
{
    this->swap_data(other);
}

template <typename T, typename A>
vector<T, A>::~vector() NESTL_NOEXCEPT_SPEC
{
    nestl::detail::destroy(m_start, m_finish);
    m_allocator.deallocate(m_start, m_end_of_storage - m_start);
}

template <typename T, typename A>
typename vector<T, A>::allocator_type
vector<T, A>::get_allocator() const NESTL_NOEXCEPT_SPEC
{
    return m_allocator;
}

template <typename T, typename A>
vector<T, A>&
vector<T, A>::operator=(vector&& other) NESTL_NOEXCEPT_SPEC
{
	move_assign(typename nestl::allocator_traits<allocator_type>::propagate_on_container_move_assignment(), std::move(other));
    return *this;
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::copy_nothrow(OperationError& err, const vector& other) NESTL_NOEXCEPT_SPEC
{
    this->assign_nothrow(err, other.cbegin(), other.cend());
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::assign_nothrow(OperationError& err, size_type n, const_reference val) NESTL_NOEXCEPT_SPEC
{
    vector tmp; tmp.swap(*this);

    assert(empty());

    grow(err, n);
    if (err)
    {
        return;
    }

    while (n > 0)
    {
        this->push_back_nothrow(err, val);
        if (err)
        {
            return;
        }
        --n;
    }
}

template <typename T, typename A>
template <typename OperationError, typename InputIterator>
void
vector<T, A>::assign_nothrow(OperationError& err, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    vector tmp; tmp.swap(*this);

    assign_iterator(err, typename std::iterator_traits<InputIterator>::iterator_category(), first, last);
}

template <typename T, typename A>
typename vector<T, A>::reference
vector<T, A>::operator[](typename vector<T, A>::size_type pos) NESTL_NOEXCEPT_SPEC
{
    assert(pos < size());
    return m_start[pos];
}

template <typename T, typename A>
typename vector<T, A>::const_reference
vector<T, A>::operator[](typename vector<T, A>::size_type pos) const NESTL_NOEXCEPT_SPEC
{
    assert(pos < size());
    return m_start[pos];
}

template <typename T, typename A>
typename vector<T, A>::reference
vector<T, A>::front() NESTL_NOEXCEPT_SPEC
{
    assert(!empty());
    return *begin();
}

template <typename T, typename A>
typename vector<T, A>::const_reference
vector<T, A>::front() const NESTL_NOEXCEPT_SPEC
{
    assert(!empty());
    return *begin();
}

template <typename T, typename A>
typename vector<T, A>::reference
vector<T, A>::back() NESTL_NOEXCEPT_SPEC
{
    assert(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename vector<T, A>::const_reference
vector<T, A>::back() const NESTL_NOEXCEPT_SPEC
{
    assert(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename vector<T, A>::pointer
vector<T, A>::data() NESTL_NOEXCEPT_SPEC
{
    assert(!empty());
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_pointer
vector<T, A>::data() const NESTL_NOEXCEPT_SPEC
{
    assert(!empty());
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
    return std::distance(m_start, m_finish);
}

template <typename T, typename A>
typename vector<T, A>::size_type
vector<T, A>::max_size() const NESTL_NOEXCEPT_SPEC
{
    return std::numeric_limits<size_type>::max();
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::reserve_nothrow(OperationError& err, size_type new_cap) NESTL_NOEXCEPT_SPEC
{
    if (new_cap <= capacity())
    {
        return;
    }
    else
    {
        if (new_cap > max_size())
        {
            build_length_error(err);
            return;
        }
    }

    do_reserve(err, new_cap);
}

template <typename T, typename A>
typename vector<T, A>::size_type
vector<T, A>::capacity() const NESTL_NOEXCEPT_SPEC
{
    return std::distance(m_start, m_end_of_storage);
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::shrink_to_fit_nothrow(OperationError& err) NESTL_NOEXCEPT_SPEC
{
    if (capacity() > size())
    {
        do_reserve(err, size());
    }
}


template <typename T, typename A>
void vector<T, A>::clear() NESTL_NOEXCEPT_SPEC
{
    nestl::detail::destroy(m_allocator, m_start, m_finish);
    m_finish = m_start;
}

template <typename T, typename A>
template <typename OperationError>
typename vector<T, A>::iterator
vector<T, A>::insert_nothrow(OperationError& err, const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return insert_value(err, pos, value);
}

template <typename T, typename A>
template <typename OperationError>
typename vector<T, A>::iterator
vector<T, A>::insert_nothrow(OperationError& err, const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	return insert_value(err, pos, std::move(value));
}

template <typename T, typename A>
template<typename OperationError, typename InputIterator>
void
vector<T, A>::insert_nothrow(OperationError& err,
                             const_iterator pos,
                             InputIterator first,
                             InputIterator last) NESTL_NOEXCEPT_SPEC
{
    insert_range(err, pos, first, last);
}


template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::push_back_nothrow(OperationError& err, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    insert_nothrow(err, cend(), value);
}


template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::push_back_nothrow(OperationError& err, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	insert_nothrow(err, cend(), std::move(value));
}

template <typename T, typename A>
template<typename OperationError, typename ... Args>
void
vector<T, A>::emplace_back_nothrow(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	insert_value(err, cend(), std::forward<Args>(args) ...);
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::resize_nothrow(OperationError& err, size_type count) NESTL_NOEXCEPT_SPEC
{
    do_resize(err, count);
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::resize_nothrow(OperationError& err, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    do_resize(err, count, value);
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

template <typename T, typename A>
void vector<T, A>::move_assign(const std::true_type& /* true_val */, vector&& other) NESTL_NOEXCEPT_SPEC
{
	const vector tmp(std::move(*this));
    this->swap_data(other);
}

template <typename T, typename A>
template <typename OperationError, typename InputIterator>
void
vector<T, A>::assign_iterator(OperationError& err,
                              std::random_access_iterator_tag /* tag */,
                              InputIterator first,
                              InputIterator last) NESTL_NOEXCEPT_SPEC
{
    assert(empty());
    size_t required_size = std::distance(first, last);

    grow(err, required_size);
    if (err)
    {
        return;
    }

    assign_iterator(err, std::input_iterator_tag(), first, last);
}

template <typename T, typename A>
template <typename OperationError, typename InputIterator>
void
vector<T, A>::assign_iterator(OperationError& err,
                              std::input_iterator_tag /* tag */,
                              InputIterator first,
                              InputIterator last) NESTL_NOEXCEPT_SPEC
{
    assert(empty());

    while (first != last)
    {
        push_back_nothrow(err, *first);
        if (err)
        {
            return;
        }

        ++first;
    }
}

template <typename T, typename A>
template <typename OperationError, typename ... Args>
typename vector<T, A>::iterator
vector<T, A>::insert_value(OperationError& err, const_iterator pos, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    assert(pos >= m_start);
    assert(pos <= m_finish);

    /// we should calculate offset before possible reallocation
    size_t offset = pos - m_start;

    if (capacity() == size())
    {
        grow(err, capacity() + 1);
        if (err)
        {
            return end();
        }
    }
    assert(capacity() > size());

    value_type* last = m_finish;
    value_type* first = m_start + offset;
    for (value_type* val = last; val != first; --val)
    {
        value_type* oldLocation = val - 1;
        nestl::class_operations::construct(err, val, *oldLocation);
        if (err)
        {
            return end();
        }
        nestl::detail::destroy(oldLocation, val);
    }


	nestl::class_operations::construct(err, first, std::forward<Args>(args) ...);
    if (err)
    {
        return end();
    }

    ++m_finish;
    return first;
}

template <typename T, typename A>
template <typename OperationError, typename InputIterator>
typename vector<T, A>::iterator
vector<T, A>::insert_range(OperationError& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(pos >= m_start);
    NESTL_ASSERT(pos <= m_finish);

    /// we should calculate offset before possible reallocation
    size_t offset = pos - m_start;

    iterator res;
    for ( ; first != last; ++first, ++offset)
    {
        const_iterator newPos = m_start + offset;
        res = insert_value(err, newPos, *first);
        if (err)
        {
            return end();
        }
    }

    return res;
}

template <typename T, typename A>
template <typename OperationError, typename ... Args>
void
vector<T, A>::do_resize(OperationError& err, size_type count, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    if (count <= size())
    {
        nestl::detail::destroy(m_start + count, m_finish);
        m_finish = m_start + count;
    }
    else
    {
        grow(err, count);
        if (err)
        {
            return;
        }

        while (m_finish < m_start + count)
        {
            nestl::class_operations::construct(err, m_finish, std::forward<Args>(args) ...);
            if (err)
            {
                return;
            }
            ++m_finish;
        }
    }
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::grow(OperationError& err, size_type requiredCapacity) NESTL_NOEXCEPT_SPEC
{
    size_t newCapacity = (((capacity() + 1) * 3) / 2);
    if (newCapacity < requiredCapacity)
    {
        newCapacity = requiredCapacity;
    }

    /// @bug overflow error
    assert(newCapacity > capacity());

    reserve_nothrow(err, newCapacity);
}

template <typename T, typename A>
template <typename OperationError>
void
vector<T, A>::do_reserve(OperationError& err, size_type new_cap) NESTL_NOEXCEPT_SPEC
{
    auto ptr = allocator_traits<allocator_type>::allocate(err, m_allocator, new_cap);
    if (err)
    {
        return;
    }
    nestl::detail::deallocation_scoped_guard<value_type*, allocator_type> guard(m_allocator, ptr, new_cap);

    nestl::detail::uninitialised_copy(err, m_start, m_finish, ptr);
    if (err)
    {
        return;
    }

    const size_t current_size = size();
    nestl::detail::destroy(m_start, m_finish);
    m_allocator.deallocate(m_start, m_end_of_storage - m_start);

    m_start = ptr;
    m_finish = ptr + current_size;
    m_end_of_storage = ptr + new_cap;

    guard.release();
}

} // namespace impl
} // namespace nestl

#endif /* NESTL_IMPLEMENTATION_VECTOR_HPP */
