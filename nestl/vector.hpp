#ifndef NESTL_VECTOR_HPP
#define NESTL_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/algorithm.hpp>
#include <nestl/memory.hpp>
#include <nestl/class_operations.hpp>
#include <nestl/operation_error.hpp>


namespace nestl
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

    typedef typename allocator_type::operation_error                        operation_error;


// constructors
    explicit vector(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC;

    explicit vector(vector&& other) NESTL_NOEXCEPT_SPEC;

// destructor
    ~vector() NESTL_NOEXCEPT_SPEC;

// allocator support
    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC;

// assignment operators and functions
    vector& operator=(vector&& other) NESTL_NOEXCEPT_SPEC;

    void copy_nothrow(operation_error& err, const vector& other) NESTL_NOEXCEPT_SPEC;

    void assign_nothrow(operation_error& err, size_type n, const_reference val = value_type()) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    void assign_nothrow(operation_error& err, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;


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

    void reserve_nothrow(operation_error& err, size_type new_cap) NESTL_NOEXCEPT_SPEC;

    size_type capacity() const NESTL_NOEXCEPT_SPEC;

    void shrink_to_fit_nothrow(operation_error& err) NESTL_NOEXCEPT_SPEC;

// modifiers
    void clear() NESTL_NOEXCEPT_SPEC;

    iterator insert_nothrow(operation_error& err, const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC;

    iterator insert_nothrow(operation_error& err, const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC;

    void insert_nothrow(operation_error& err, const_iterator pos, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template<typename InputIterator>
    void insert_nothrow(operation_error& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    template<typename ... Args>
    iterator emplace_nothrow(operation_error& err, const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC;

    iterator erase_nothrow(operation_error& err, const_iterator pos) NESTL_NOEXCEPT_SPEC;

    iterator erase_nothrow(operation_error& err, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    void push_back_nothrow(operation_error& err, const value_type& value) NESTL_NOEXCEPT_SPEC;

    void push_back_nothrow(operation_error& err, value_type&& value) NESTL_NOEXCEPT_SPEC;

    template<typename ... Args>
    void emplace_back_nothrow(operation_error& err, Args&&... args) NESTL_NOEXCEPT_SPEC;

    void pop_back() NESTL_NOEXCEPT_SPEC;

    void resize_nothrow(operation_error& err, size_type count) NESTL_NOEXCEPT_SPEC;

    void resize_nothrow(operation_error& err, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    void swap(vector& other) NESTL_NOEXCEPT_SPEC;

private:
    allocator_type m_allocator;

    pointer m_start;
    pointer m_finish;
    pointer m_end_of_storage;

    void swap_data(vector& other) NESTL_NOEXCEPT_SPEC;

    void move_assign(const std::true_type& /* true_val */, vector&& other) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    void assign_iterator(operation_error& err,
                         std::random_access_iterator_tag /* tag */,
                         InputIterator first,
                         InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    void assign_iterator(operation_error& err,
                         std::input_iterator_tag /* tag */,
                         InputIterator first,
                         InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename ... Args>
    iterator insert_value(operation_error& err, const_iterator pos, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    iterator insert_range(operation_error& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    template <typename ... Args>
    void do_resize(operation_error& err, size_type count, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    void grow(operation_error& err, size_type requiredCapacity) NESTL_NOEXCEPT_SPEC;

    void do_reserve(operation_error& err, size_type new_cap) NESTL_NOEXCEPT_SPEC;
};

/**
 * @brief specialization of class_operations for nestl::vector
 *
 * Allows construct one vector from another (emulate copy construction)
 */
template <typename T, typename VectorAllocator>
struct class_operations <nestl::vector<T, VectorAllocator> >
{
    typedef nestl::vector<T, VectorAllocator> vector_t;

    template <typename Allocator>
    static void construct(typename Allocator::operation_error& err, vector_t* ptr, Allocator& alloc, const vector_t& other) NESTL_NOEXCEPT_SPEC
    {
        allocator_traits<Allocator>::construct(err, alloc, ptr);
        if (err)
        {
            return;
        }

        vector_t* end = ptr + 1;
        nestl::detail::destruction_scoped_guard<vector_t*, Allocator> guard(ptr, end, alloc);

        ptr->copy_nothrow(err, other);
        if (err)
        {
            return;
        }

        guard.release();
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
    nestl::detail::destroy(m_allocator, m_start, m_finish);
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
void
vector<T, A>::copy_nothrow(operation_error& err, const vector& other) NESTL_NOEXCEPT_SPEC
{
    this->assign_nothrow(err, other.cbegin(), other.cend());
}

template <typename T, typename A>
void
vector<T, A>::assign_nothrow(operation_error& err, size_type n, const_reference val) NESTL_NOEXCEPT_SPEC
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
template <typename InputIterator>
void
vector<T, A>::assign_nothrow(operation_error& err, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
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
    return nestl::distance(m_start, m_finish);
}

template <typename T, typename A>
typename vector<T, A>::size_type
vector<T, A>::max_size() const NESTL_NOEXCEPT_SPEC
{
    return nestl::allocator_traits<A>::max_size(m_allocator);
}

template <typename T, typename A>
void
vector<T, A>::reserve_nothrow(operation_error& err, size_type new_cap) NESTL_NOEXCEPT_SPEC
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
    return nestl::distance(m_start, m_end_of_storage);
}

template <typename T, typename A>
void
vector<T, A>::shrink_to_fit_nothrow(operation_error& err) NESTL_NOEXCEPT_SPEC
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
typename vector<T, A>::iterator
vector<T, A>::insert_nothrow(operation_error& err, const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return insert_value(err, pos, value);
}

template <typename T, typename A>
typename vector<T, A>::iterator
vector<T, A>::insert_nothrow(operation_error& err, const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	return insert_value(err, pos, std::move(value));
}

template <typename T, typename A>
template<typename InputIterator>
void
vector<T, A>::insert_nothrow(operation_error& err,
                             const_iterator pos,
                             InputIterator first,
                             InputIterator last) NESTL_NOEXCEPT_SPEC
{
    insert_range(err, pos, first, last);
}


template <typename T, typename A>
void
vector<T, A>::push_back_nothrow(operation_error& err, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    insert_nothrow(err, cend(), value);
}


template <typename T, typename A>
void
vector<T, A>::push_back_nothrow(operation_error& err, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	insert_nothrow(err, cend(), std::move(value));
}

template <typename T, typename A>
template<typename ... Args>
void
vector<T, A>::emplace_back_nothrow(operation_error& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	insert_value(err, cend(), std::forward<Args>(args) ...);
}

template <typename T, typename A>
void
vector<T, A>::resize_nothrow(operation_error& err, size_type count) NESTL_NOEXCEPT_SPEC
{
    do_resize(err, count);
}

template <typename T, typename A>
void
vector<T, A>::resize_nothrow(operation_error& err, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC
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
template <typename InputIterator>
void
vector<T, A>::assign_iterator(operation_error& err,
                              std::random_access_iterator_tag /* tag */,
                              InputIterator first,
                              InputIterator last) NESTL_NOEXCEPT_SPEC
{
    assert(empty());
    size_t required_size = nestl::distance(first, last);

    grow(err, required_size);
    if (err)
    {
        return;
    }

    assign_iterator(err, std::input_iterator_tag(), first, last);
}

template <typename T, typename A>
template <typename InputIterator>
void
vector<T, A>::assign_iterator(operation_error& err,
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
template <typename ... Args>
typename vector<T, A>::iterator
vector<T, A>::insert_value(operation_error& err, const_iterator pos, Args&& ... args) NESTL_NOEXCEPT_SPEC
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
        nestl::detail::construct(err, val, m_allocator, *oldLocation);
        if (err)
        {
            return end();
        }
        nestl::detail::destroy(m_allocator, oldLocation, val);
    }


	nestl::detail::construct(err, first, m_allocator, std::forward<Args>(args) ...);
    if (err)
    {
        return end();
    }

    ++m_finish;
    return first;
}

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::iterator
vector<T, A>::insert_range(operation_error& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
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
template <typename ... Args>
void
vector<T, A>::do_resize(operation_error& err, size_type count, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    if (count <= size())
    {
        nestl::detail::destroy(m_allocator, m_start + count, m_finish);
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
            nestl::detail::construct(err, m_finish, m_allocator, std::forward<Args>(args) ...);
            if (err)
            {
                return;
            }
            ++m_finish;
        }
    }
}

template <typename T, typename A>
void
vector<T, A>::grow(operation_error& err, size_type requiredCapacity) NESTL_NOEXCEPT_SPEC
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
void
vector<T, A>::do_reserve(operation_error& err, size_type new_cap) NESTL_NOEXCEPT_SPEC
{
    auto ptr = allocator_traits<allocator_type>::allocate(err, m_allocator, new_cap);
    if (err)
    {
        return;
    }
    nestl::detail::allocation_scoped_guard<value_type*, allocator_type> guard(m_allocator, ptr, new_cap);

    nestl::uninitialised_copy(err, m_start, m_finish, ptr, m_allocator);
    if (err)
    {
        return;
    }

    const size_t current_size = size();
    nestl::detail::destroy(m_allocator, m_start, m_finish);
    m_allocator.deallocate(m_start, m_end_of_storage - m_start);

    m_start = ptr;
    m_finish = ptr + current_size;
    m_end_of_storage = ptr + new_cap;

    guard.release();
}

} // namespace nestl

#endif /* NESTL_VECTOR_HPP */
