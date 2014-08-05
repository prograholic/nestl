#ifndef NESTL_VECTOR_HPP
#define NESTL_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/noncopyable.hpp>

#include <nestl/detail/construct.hpp>

#include <system_error>


#include <vector>
#include <initializer_list>
#include <iterator>

namespace nestl
{

/**
 * @brief
 */
template <typename T, typename Allocator = nestl::allocator<T>>
class vector : private nestl::noncopyable
{
public:

    typedef T                                                               value_type;
    typedef Allocator                                                       allocator_type;
    typedef std::size_t                                                     size_type;
    typedef std::ptrdiff_t                                                  difference_type;
    typedef T&                                                              reference;
    typedef const T&                                                        const_reference;
    typedef typename std::allocator_traits<allocator_type>::pointer         pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer   const_pointer;

    typedef pointer                                                         iterator;
    typedef const_pointer                                                   const_iterator;
    typedef std::reverse_iterator<iterator>                                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                           const_reverse_iterator;

    typedef std::error_condition                                            operation_error;


// constructors
    explicit vector(const allocator_type& alloc = allocator_type()) noexcept;

    explicit vector(vector&& other) noexcept;


// destructor
    ~vector() noexcept;

// allocator support
    allocator_type get_allocator() const noexcept;

// assignment operators and functions
    vector& operator=(vector&& other) noexcept;

    operation_error assign_copy(const vector& other) noexcept;

    operation_error assign(size_type n, const_reference val = value_type()) noexcept;

    template <typename InputIterator>
    operation_error assign(InputIterator first, InputIterator last) noexcept;

    operation_error assign(std::initializer_list<value_type> ilist) noexcept;



// element access
    reference operator[](size_type pos) noexcept;

    const_reference operator[](size_type pos) const noexcept;

    reference front() noexcept;

    const_reference front() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

    pointer data() noexcept;

    const_pointer data() const noexcept;

// iterators
    iterator begin() noexcept;

    const_iterator begin() const noexcept;

    const_iterator cbegin() const noexcept;

    iterator end() noexcept;

    const_iterator end() const noexcept;

    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;

    const_reverse_iterator rbegin() const noexcept;

    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;

    const_reverse_iterator rend() const noexcept;

    const_reverse_iterator crend() const noexcept;

// capacity
    bool empty() const noexcept;

    size_type size() const noexcept;

    size_type max_size() const noexcept;

    operation_error reserve(size_type new_cap) noexcept;

    size_type capacity() const noexcept;

    operation_error shrink_to_fit() noexcept;

// modifiers
    void clear() noexcept;

    operation_error insert(const_iterator pos, const value_type& value) noexcept;

    operation_error insert(const_iterator pos, value_type&& value) noexcept;

    operation_error insert(const_iterator pos, size_type count, const value_type& value) noexcept;

    template<typename InputIterator>
    operation_error insert(const_iterator pos, InputIterator first, InputIterator last) noexcept;

    operation_error insert(const_iterator pos, std::initializer_list<T> ilist) noexcept;

    template<typename ... Args>
    operation_error emplace(const_iterator pos, Args&&... args) noexcept;

    operation_error erase(const_iterator pos) noexcept;

    operation_error erase(const_iterator first, const_iterator last) noexcept;

    operation_error push_back(const value_type& value) noexcept;

    operation_error push_back(value_type&& value) noexcept;

    template<typename ... Args>
    operation_error emplace_back(Args&& ... args) noexcept;

    void pop_back() noexcept;

    operation_error resize(size_type count) noexcept;

    operation_error resize(size_type count, const value_type& value) noexcept;

    void swap(vector& other) noexcept;

private:
    allocator_type m_allocator;

    pointer m_start;
    pointer m_finish;
    pointer m_end_of_storage;

    void swap_data(vector& other) noexcept;

    void move_assign(const std::true_type& /* true_val */, vector&& other) noexcept;

    template <typename InputIterator>
    operation_error assign_iterator(std::random_access_iterator_tag /* tag */, InputIterator first, InputIterator last) noexcept;

    template <typename InputIterator>
    operation_error assign_iterator(std::input_iterator_tag /* tag */, InputIterator first, InputIterator last) noexcept;
};



/// Implementation

template <typename T, typename A>
vector<T, A>::vector(const allocator_type& alloc) noexcept
    : m_allocator(alloc)
    , m_start(0)
    , m_finish(0)
    , m_end_of_storage(0)
{
}

template <typename T, typename A>
vector<T, A>::vector(vector&& other) noexcept
    : m_allocator(std::move(other.get_allocator()))
    , m_start(0)
    , m_finish(0)
    , m_end_of_storage(0)
{
    this->swap_data(other);
}

template <typename T, typename A>
vector<T, A>::~vector() noexcept
{
    nestl::detail::destroy(m_allocator, m_start, m_finish);
    m_allocator.deallocate(m_start, m_end_of_storage - m_start);
}

template <typename T, typename A>
typename vector<T, A>::allocator_type vector<T, A>::get_allocator() const noexcept
{
    return m_allocator;
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(vector&& other) noexcept
{
    move_assign(typename std::allocator_traits<A>::propagate_on_container_move_assignment(), std::move(other));
    return *this;
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::assign_copy(const vector& other) noexcept
{
    return this->assign(other.cbegin(), other.cend());
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::assign(size_type n, const_reference val) noexcept
{
    const vector tmp(std::move(*this));

    NESTL_ASSERT(empty());

    auto err = reserve(n);
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


    NESTL_ASSERT(0 && "not implemented");

    return operation_error(std::errc::not_enough_memory);
}

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::operation_error vector<T, A>::assign(InputIterator first, InputIterator last) noexcept
{
    const vector tmp(std::move(*this));

    return assign_iterator(typename std::iterator_traits<InputIterator>::iterator_category(), first, last);
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::assign(std::initializer_list<typename vector<T, A>::value_type> ilist) noexcept
{
    return this->assign(ilist.begin(), ilist.end());
}

template <typename T, typename A>
typename vector<T, A>::reference vector<T, A>::operator[](typename vector<T, A>::size_type pos) noexcept
{
    NESTL_ASSERT(pos < size());
    return m_start[pos];
}

template <typename T, typename A>
typename vector<T, A>::const_reference vector<T, A>::operator[](typename vector<T, A>::size_type pos) const noexcept
{
    NESTL_ASSERT(pos < size());
    return m_start[pos];
}

template <typename T, typename A>
typename vector<T, A>::reference vector<T, A>::front() noexcept
{
    NESTL_ASSERT(!empty());
    return *begin();
}

template <typename T, typename A>
typename vector<T, A>::const_reference vector<T, A>::front() const noexcept
{
    assert(!empty());
    return *begin();
}

template <typename T, typename A>
typename vector<T, A>::reference vector<T, A>::back() noexcept
{
    NESTL_ASSERT(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename vector<T, A>::const_reference vector<T, A>::back() const noexcept
{
    NESTL_ASSERT(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename vector<T, A>::pointer vector<T, A>::data() noexcept
{
    NESTL_ASSERT(!empty());
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_pointer vector<T, A>::data() const noexcept
{
    NESTL_ASSERT(!empty());
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::iterator vector<T, A>::begin() noexcept
{
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator vector<T, A>::begin() const noexcept
{
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator vector<T, A>::cbegin() const noexcept
{
    return m_start;
}

template <typename T, typename A>
typename vector<T, A>::iterator vector<T, A>::end() noexcept
{
    return m_finish;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator vector<T, A>::end() const noexcept
{
    return m_finish;
}

template <typename T, typename A>
typename vector<T, A>::const_iterator vector<T, A>::cend() const noexcept
{
    return m_finish;
}

template <typename T, typename A>
typename vector<T, A>::reverse_iterator vector<T, A>::rbegin() noexcept
{
    return reverse_iterator(m_finish);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator vector<T, A>::rbegin() const noexcept
{
    return const_reverse_iterator(m_finish);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator vector<T, A>::crbegin() const noexcept
{
    return const_reverse_iterator(m_finish);
}

template <typename T, typename A>
typename vector<T, A>::reverse_iterator vector<T, A>::rend() noexcept
{
    return reverse_iterator(m_start);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator vector<T, A>::rend() const noexcept
{
    return const_reverse_iterator(m_start);
}

template <typename T, typename A>
typename vector<T, A>::const_reverse_iterator vector<T, A>::crend() const noexcept
{
    return const_reverse_iterator(m_start);
}

template <typename T, typename A>
bool vector<T, A>::empty() const noexcept
{
    return m_start == m_finish;
}

template <typename T, typename A>
typename vector<T, A>::size_type vector<T, A>::size() const noexcept
{
    return std::distance(m_start, m_finish);
}

template <typename T, typename A>
typename vector<T, A>::size_type vector<T, A>::max_size() const noexcept
{
    return m_allocator.max_size();
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::reserve(size_type new_cap) noexcept
{
    if (new_cap <= capacity())
    {
        return operation_error();
    }
    NESTL_ASSERT(0 && "not implemented");

    /// @todo return value_too_large for new_cap > max_size()
    return operation_error(std::errc::not_enough_memory);
}

template <typename T, typename A>
typename vector<T, A>::size_type vector<T, A>::capacity() const noexcept
{
    return std::distance(m_start, m_end_of_storage);
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::shrink_to_fit() noexcept
{
    NESTL_ASSERT(0 && "not implemented");

    return operation_error(std::errc::not_enough_memory);
}


template <typename T, typename A>
void vector<T, A>::clear() noexcept
{
    NESTL_ASSERT(0 && "not implemented");
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::insert(const_iterator pos, const value_type& value) noexcept
{
    NESTL_ASSERT(0 && "not implemented");

    return operation_error(std::errc::not_enough_memory);
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::insert(const_iterator pos, value_type&& value) noexcept
{
    NESTL_ASSERT(0 && "not implemented");

    return operation_error(std::errc::not_enough_memory);
}


template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::push_back(const value_type& value) noexcept
{
    return this->insert(this->cend(), value);
}

template <typename T, typename A>
typename vector<T, A>::operation_error vector<T, A>::push_back(value_type&& value) noexcept
{
    return this->insert(this->cend(), std::move(value));
}

/// Private implementation

template <typename T, typename A>
void vector<T, A>::swap_data(vector& other) noexcept
{
    std::swap(this->m_start, other.m_start);
    std::swap(this->m_finish, other.m_finish);
    std::swap(this->m_end_of_storage, other.m_end_of_storage);
}

template <typename T, typename A>
void vector<T, A>::move_assign(const std::true_type& /* true_val */, vector&& other) noexcept
{
    const vector tmp(std::move(*this));
    this->swap_data(other);
}

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::operation_error vector<T, A>::assign_iterator(std::random_access_iterator_tag /* tag */, InputIterator first, InputIterator last) noexcept
{
    NESTL_ASSERT(empty());
    size_t required_size = std::distance(first, last);

    auto err = this->reserve(required_size);
    if (err)
    {
        return err;
    }

    return this->assign_iterator(std::input_iterator_tag(), first, last);
}

template <typename T, typename A>
template <typename InputIterator>
typename vector<T, A>::operation_error vector<T, A>::assign_iterator(std::input_iterator_tag /* tag */, InputIterator first, InputIterator last) noexcept
{
    NESTL_ASSERT(empty());

    while (first != last)
    {
        auto err = this->push_back(*first);
        if (err)
        {
            return err;
        }

        ++first;
    }

    return operation_error();
}


} // namespace nestl

#endif /* NESTL_VECTOR_HPP */
