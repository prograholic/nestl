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
#include <nestl/swap.hpp>


namespace nestl
{
    
template <typename T, typename Allocator>
class vector_base : private Allocator
{
public:
   
    typedef T                                                               value_type;
    typedef Allocator                                                       allocator_type;
    typedef nestl::size_t                                                   size_type;
    typedef nestl::ptrdiff_t                                                difference_type;
    typedef T&                                                              reference;
    typedef const T&                                                        const_reference;
    typedef typename nestl::allocator_traits<allocator_type>::pointer       pointer;
    typedef typename nestl::allocator_traits<allocator_type>::const_pointer const_pointer;

    typedef pointer                                                         iterator;
    typedef const_pointer                                                   const_iterator;
    typedef nestl::reverse_iterator<iterator>                               reverse_iterator;
    typedef nestl::reverse_iterator<const_iterator>                         const_reverse_iterator;
    
    typedef nestl::allocator_traits<Allocator>                              alloc_traits;

    vector_base(const allocator_type& alloc) noexcept
        : Allocator(alloc)
        , m_start(0)
        , m_finish(0)
        , m_end_of_storage(0)
    {
    }

    /// @todo move to nothrow version of vector_base
    vector_base(vector_base&& other) noexcept
        : Allocator(nestl::move(other.get_alloc()))
        , m_start(0)
        , m_finish(0)
        , m_end_of_storage(0)
    {
        this->swap_data(other);
    }

    ~vector_base() noexcept
    {
        nestl::detail::destroy(get_alloc(), m_start, m_finish);
        alloc_traits::deallocate(get_alloc(), m_start, m_end_of_storage - m_start);
    }
    
    void swap_data(vector_base& other) noexcept
    {
        nestl::swap(this->m_start, other.m_start);
        nestl::swap(this->m_finish, other.m_finish);
        nestl::swap(this->m_end_of_storage, other.m_end_of_storage);
    }
    
    allocator_type& get_alloc() noexcept
    {
        return static_cast<allocator_type&>(*this);
    }
    
    const allocator_type& get_alloc() const noexcept
    {
        return static_cast<const allocator_type&>(*this);
    }
    
    void clear() noexcept
    {
        nestl::detail::destroy(get_alloc(), m_start, m_finish);
        m_finish = m_start;
    }

public:
    
    pointer m_start;
    pointer m_finish;
    pointer m_end_of_storage;
};

template <typename VectorBase>
struct vector_base_nothrow : public VectorBase
{
    
};

/**
 * @brief
 */
template <typename T, typename Allocator = nestl::allocator<T> >
class vector : private vector_base<T, Allocator>
{
public:

    typedef vector_base<T, Allocator>                                              base_type;
    
    typedef typename base_type::value_type                                         value_type;
    typedef typename base_type::allocator_type                                     allocator_type;
    typedef typename base_type::size_type                                          size_type;
    typedef typename base_type::difference_type                                    difference_type;
    typedef typename base_type::reference                                          reference;
    typedef typename base_type::const_reference                                    const_reference;
    typedef typename base_type::pointer                                            pointer;
    typedef typename base_type::const_pointer                                      const_pointer;
    typedef typename base_type::iterator                                           iterator;
    typedef typename base_type::const_iterator                                     const_iterator;
    typedef typename base_type::reverse_iterator                                   reverse_iterator;
    typedef typename base_type::const_reverse_iterator                             const_reverse_iterator;
    typedef nestl::error_condition                                                 operation_error;

    typedef typename nestl::result_with_operation_error<iterator, operation_error> iterator_with_operation_error;

    typedef typename base_type::alloc_traits                                       alloc_traits;


// constructors
    explicit vector(const allocator_type& alloc = allocator_type()) noexcept
        : base_type(alloc)
    {
    }

    explicit vector(vector&& other) noexcept
        : base_type(nestl::move(other))
    {
    }
    
public: // allocator support
    
    /// @todo move to nothrow or throw version of vector_base
    allocator_type get_allocator() const noexcept
    {
        return allocator_type(this->get_alloc());
    }
    
public: // data accessors

    reference operator[](size_type pos) noexcept
    {
        NESTL_ASSERT(pos < size());
        return this->m_start[pos];
    }

    const_reference operator[](size_type pos) const noexcept
    {
        NESTL_ASSERT(pos < size());
        return this->m_start[pos];
    }

    reference front() noexcept
    {
        NESTL_ASSERT(!empty());
        return *begin();
    }

    const_reference front() const noexcept
    {
        NESTL_ASSERT(!empty());
        return *begin();
    }

    reference back() noexcept
    {
        NESTL_ASSERT(!empty());
        return *rbegin();
    }

    const_reference back() const noexcept
    {
        NESTL_ASSERT(!empty());
        return *rbegin();
    }

    pointer data() noexcept
    {
        NESTL_ASSERT(!empty());
        return this->m_start;
    }

    const_pointer data() const noexcept
    {
        NESTL_ASSERT(!empty());
        return this->m_start;
    }

    iterator begin() noexcept
    {
        return this->m_start;
    }

    const_iterator begin() const noexcept
    {
        return this->m_start;
    }

    const_iterator cbegin() const noexcept
    {
        return this->m_start;
    }

    iterator end() noexcept
    {
        return this->m_finish;
    }

    const_iterator end() const noexcept
    {
        return this->m_finish;
    }

    const_iterator cend() const noexcept
    {
        return this->m_finish;
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(this->m_finish);
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(this->m_finish);
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(this->m_finish);
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(this->m_start);
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(this->m_start);
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(this->m_start);
    }

    bool empty() const noexcept
    {
        return this->m_start == this->m_finish;
    }

    size_type size() const noexcept
    {
        return static_cast<size_type>(nestl::distance(this->m_start, this->m_finish));
    }

    size_type max_size() const noexcept
    {
        return alloc_traits::max_size(this->get_alloc());
    }
    
    size_type capacity() const noexcept
    {
        return static_cast<size_type>(nestl::distance(this->m_start, this->m_end_of_storage));
    }
    
    

// assignment operators and functions
    vector& operator=(vector&& other) noexcept
    {
        move_assign(typename alloc_traits::propagate_on_container_move_assignment(), nestl::move(other));
        return *this;
    }

    void assign_copy(operation_error& error, const vector& other) noexcept
    {
        return this->assign_nothrow(error, other.cbegin(), other.cend());
    }

    void assign_nothrow(operation_error& error, size_type n, const_reference val = value_type()) noexcept
    {
        vector tmp; tmp.swap(*this);

        NESTL_ASSERT(empty());

        grow_nothrow(error, n);
        if (error)
        {
            return;
        }

        while (n > 0)
        {
            this->push_back_nothrow(error, val);
            if (error)
            {
                return;
            }
            --n;
        }
    }

    template<typename InputIterator>
    void assign_nothrow(operation_error& error, InputIterator first, InputIterator last) noexcept
    {
        vector tmp; tmp.swap(*this);

        assign_iterator_nothrow(error, typename nestl::iterator_traits<InputIterator>::iterator_category(), first, last);
    }




// modifiers

    void reserve_nothrow(operation_error& error, size_type new_cap) noexcept
    {
        if (new_cap <= capacity())
        {
            return;
        }
        else
        {
            if (new_cap > max_size())
            {
                error = operation_error(nestl::errc::value_too_large);
                return;
            }
        }

        do_reserve_nothrow(error, new_cap);
    }

    void shrink_to_fit_nothrow(operation_error& error) noexcept
    {
        if (capacity() > size())
        {
            return do_reserve_nothrow(error, size());
        }

        return;
    }

    void clear() noexcept
    {
        base_type::clear();
    }

    iterator insert_nothrow(operation_error& error, const_iterator pos, const value_type& value) noexcept
    {
        return insert_value_nothrow(error, pos, value);
    }

    iterator insert_nothrow(operation_error& error, const_iterator pos, value_type&& value) noexcept
    {
        return insert_value_nothrow(error, pos, nestl::move(value));
    }

    template<typename InputIterator>
    void insert_nothrow(operation_error& error, const_iterator pos, InputIterator first, InputIterator last) noexcept
    {
        insert_range_nothrow(error, pos, first, last);
    }

    void push_back_nothrow(operation_error& error, const value_type& value) noexcept
    {
        insert_nothrow(error, cend(), value);
    }

    void push_back_nothrow(operation_error& error, value_type&& value) noexcept
    {
        insert_nothrow(error, cend(), nestl::move(value));
    }

    template<typename ... Args>
    void emplace_back_nothrow(operation_error& error, Args&& ... args) noexcept
    {
        insert_value_nothrow(error, cend(), nestl::forward<Args>(args) ...);
    }

    void resize_nothrow(operation_error& error, size_type count) noexcept
    {
        do_resize_nothrow(error, count);
    }

    void resize_nothrow(operation_error& error, size_type count, const value_type& value) noexcept
    {
        do_resize_nothrow(error, count, value);
    }

    void swap(vector& other) noexcept
    {
        nestl::swap(this->get_alloc(), other.get_alloc());
        this->swap_data(other);
    }

private:

    void move_assign(const nestl::true_type& /* true_val */, vector&& other) noexcept
    {
        const vector tmp(nestl::move(*this));
        this->swap_data(other);
    }

    template <typename InputIterator>
    void assign_iterator_nothrow(operation_error& error,
                                 nestl::random_access_iterator_tag /* tag */,
                                 InputIterator first,
                                 InputIterator last) noexcept
    {
        NESTL_ASSERT(empty());
        size_t required_size = nestl::distance(first, last);

        grow_nothrow(error, required_size);
        if (error)
        {
            return;
        }

        assign_iterator_nothrow(error, nestl::input_iterator_tag(), first, last);
    }

    template <typename InputIterator>
    void assign_iterator_nothrow(operation_error& error,
                                 nestl::input_iterator_tag /* tag */,
                                 InputIterator first,
                                 InputIterator last) noexcept
    {
        NESTL_ASSERT(empty());

        while (first != last)
        {
            push_back_nothrow(error, *first);
            if (error)
            {
                return;
            }

            ++first;
        }
    }

    template <typename ... Args>
    iterator insert_value_nothrow(operation_error& error, const_iterator pos, Args&& ... args) noexcept
    {
        NESTL_ASSERT(pos >= this->m_start);
        NESTL_ASSERT(pos <= this->m_finish);

        /// we should calculate offset before possible reallocation
        size_t offset = pos - this->m_start;

        if (capacity() == size())
        {
            grow_nothrow(error, capacity() + 1);
            if (error)
            {
                return iterator();
            }
        }
        NESTL_ASSERT(capacity() > size());

        value_type* last = this->m_finish;
        value_type* first = this->m_start + offset;
        for (value_type* val = last; val != first; --val)
        {
            value_type* oldLocation = val - 1;
            error = nestl::detail::construct<operation_error>(val, this->get_alloc(), *oldLocation);
            if (error)
            {
                return iterator();
            }
            nestl::detail::destroy(this->get_alloc(), oldLocation, val);
        }


        error = nestl::detail::construct<operation_error>(first, this->get_alloc(), nestl::forward<Args>(args) ...);
        if (error)
        {
            return iterator();
        }

        ++this->m_finish;
        return first;
    }

    template <typename InputIterator>
    iterator insert_range_nothrow(operation_error& error, const_iterator pos, InputIterator first, InputIterator last) noexcept
    {
        NESTL_ASSERT(pos >= this->m_start);
        NESTL_ASSERT(pos <= this->m_finish);

        /// we should calculate offset before possible reallocation
        size_t offset = pos - this->m_start;

        iterator res = begin() + distance(cbegin(), pos);
        for ( ; first != last; ++first, ++offset)
        {
            const_iterator newPos = this->m_start + offset;
            res = insert_value_nothrow(error, newPos, *first);
            if (error)
            {
                return res;
            }
        }

        return res;
    }

    template <typename ... Args>
    void do_resize_nothrow(operation_error& error, size_type count, Args&& ... args) noexcept
    {
        if (count <= size())
        {
            nestl::detail::destroy(this->get_alloc(), this->m_start + count, this->m_finish);
            this->m_finish = this->m_start + count;
        }
        else
        {
            grow_nothrow(error, count);
            if (error)
            {
                return;
            }

            while (this->m_finish < this->m_start + count)
            {
                error = nestl::detail::construct<operation_error>(this->m_finish, this->get_alloc(), nestl::forward<Args>(args) ...);
                if (error)
                {
                    return;
                }
                ++this->m_finish;
            }
        }
    }

    void grow_nothrow(operation_error& error, size_type requiredCapacity) noexcept
    {
        size_type newCapacity = (((capacity() + 1) * 3) / 2);
        if (newCapacity < requiredCapacity)
        {
            newCapacity = requiredCapacity;
        }

        /// @bug overflow error
        NESTL_ASSERT(newCapacity > capacity());

        reserve_nothrow(error, newCapacity);
    }

    void do_reserve_nothrow(operation_error& error, size_type new_cap) noexcept
    {
        value_type* ptr = alloc_traits::allocate(this->get_alloc(), new_cap);
        if (!ptr)
        {
            error = operation_error(nestl::errc::not_enough_memory);
            return;
        }
        nestl::detail::allocation_scoped_guard<value_type*, allocator_type> guard(this->get_alloc(), ptr, new_cap);

        error = nestl::uninitialised_copy<operation_error>(this->m_start, this->m_finish, ptr, this->get_alloc());
        if (error)
        {
            return;
        }

        const size_t current_size = size();
        nestl::detail::destroy(this->get_alloc(), this->m_start, this->m_finish);
        alloc_traits::deallocate(this->get_alloc(), this->m_start, this->m_end_of_storage - this->m_start);

        this->m_start = ptr;
        this->m_finish = ptr + current_size;
        this->m_end_of_storage = ptr + new_cap;

        guard.release();
    }
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
    static OperationError construct(vector_t* ptr, Allocator& alloc) noexcept
    {
        alloc.construct(ptr);

        return OperationError();
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(vector_t* ptr, Allocator& alloc, vector_t&& other) noexcept
    {
        alloc.construct(ptr, nestl::move(other));

        return OperationError();
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(vector_t* ptr, Allocator& alloc, const vector_t& other) noexcept
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
                  const nestl::vector<T, Allocator>& right) noexcept
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


} // namespace nestl

#endif /* NESTL_VECTOR_HPP */
