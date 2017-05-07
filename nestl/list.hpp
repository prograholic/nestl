/**
 * @file list.hpp - implementation of nestl::list container
 *
 * @note Implementation based on implementation of std::list from libstdc++
 */

#ifndef NESTL_LIST_HPP
#define NESTL_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/memory.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/operation_error.hpp>
#include <nestl/algorithm.hpp>
#include <nestl/system_error.hpp>
#include <nestl/alignment.hpp>
#include <nestl/functional.hpp>

#include <cassert>

namespace nestl
{


namespace detail
{


#ifndef NDEBUG

#define NESTL_CHECK_LIST_NODE(node) \
    do \
    { \
        assert(node); \
        assert((node)->m_prev); \
        assert((node)->m_next); \
        assert((node)->m_prev->m_next == (node)); \
        assert((node)->m_next->m_prev == (node)); \
    } \
    while ((void)0,0)\

#else /* NDEBUG */

#define NESTL_CHECK_LIST_NODE(node)

#endif /* NDEBUG */

struct list_node_base
{
    list_node_base* m_next;
    list_node_base* m_prev;

    static void swap(list_node_base& left, list_node_base& right) NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(&left);
        NESTL_CHECK_LIST_NODE(&right);

        if (left.m_next != &left)
        {
            if (right.m_next != &right)
            {
                // Both left and right are not empty.
                std::swap(left.m_next,right.m_next);
				std::swap(left.m_prev, right.m_prev);
                left.m_next->m_prev = left.m_prev->m_next = &left;
                right.m_next->m_prev = right.m_prev->m_next = &right;
            }
            else
            {
                // left is not empty, right is empty.
                right.m_next = left.m_next;
                right.m_prev = left.m_prev;
                right.m_next->m_prev = right.m_prev->m_next = &right;
                left.m_next = left.m_prev = &left;
            }
        }
        else if (right.m_next != &right)
        {
            // left is empty, right is not empty.
            left.m_next = right.m_next;
            left.m_prev = right.m_prev;
            left.m_next->m_prev = left.m_prev->m_next = &left;
            right.m_next = right.m_prev = &right;
        }

        NESTL_CHECK_LIST_NODE(&left);
        NESTL_CHECK_LIST_NODE(&right);
    }

    void init_empty() NESTL_NOEXCEPT_SPEC
    {
        m_next = this;
        m_prev = this;

        NESTL_CHECK_LIST_NODE(this);
    }

    void inject(list_node_base* node) NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(this);
        NESTL_CHECK_LIST_NODE(node);

        this->m_next = node;
        this->m_prev = node->m_prev;
        node->m_prev->m_next = this;
        node->m_prev = this;

        NESTL_CHECK_LIST_NODE(this);
        NESTL_CHECK_LIST_NODE(node);
    }

    void remove() NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(this);

        list_node_base* next_node = this->m_next;
        NESTL_CHECK_LIST_NODE(next_node);

        list_node_base* prev_node = this->m_prev;
        NESTL_CHECK_LIST_NODE(prev_node);

        prev_node->m_next = next_node;
        next_node->m_prev = prev_node;

        /**
         * @note we do not check `this`, because after `remove` `this` usually will be destroyed
         */
        ///NESTL_CHECK_LIST_NODE(this);

        NESTL_CHECK_LIST_NODE(next_node);
        NESTL_CHECK_LIST_NODE(prev_node);
    }


    void transfer(list_node_base* first, list_node_base* last) NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(this);
        NESTL_CHECK_LIST_NODE(first);
        NESTL_CHECK_LIST_NODE(last);

        assert(this != last);

        last->m_prev->m_next = this;
        first->m_prev->m_next = last;
        this->m_prev->m_next = first;

        list_node_base* oldPrev = this->m_prev;
        this->m_prev = last->m_prev;
        last->m_prev = first->m_prev;
        first->m_prev = oldPrev;

        NESTL_CHECK_LIST_NODE(this);
        NESTL_CHECK_LIST_NODE(first);
        NESTL_CHECK_LIST_NODE(last);
    }
};


template <typename T>
struct list_node : public list_node_base
{
    nestl::aligned_buffer<T> m_value;

    list_node() NESTL_NOEXCEPT_SPEC
        : list_node_base()
    {
    }

    template <typename Allocator>
    void destroy_value(Allocator& a)
    {
        T* value = get_pointer();
        nestl::detail::destroy(a, value, value + 1);
    }

    template <typename OperationError, typename Allocator, typename ... Args>
    OperationError initialize(Allocator& a, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		auto err = nestl::detail::construct<OperationError>(get_pointer(), a, std::forward<Args>(args) ...);
        return err;
    }

    T* get_pointer() NESTL_NOEXCEPT_SPEC
    {
        return static_cast<T*>(static_cast<void*>(std::addressof(m_value)));
    }

    T& get_reference() NESTL_NOEXCEPT_SPEC
    {
        return *get_pointer();
    }


    const T* get_pointer() const NESTL_NOEXCEPT_SPEC
    {
        return static_cast<const T*>(static_cast<const void*>(std::addressof(m_value)));
    }

    const T& get_reference() const NESTL_NOEXCEPT_SPEC
    {
        return *get_pointer();
    }
};


template <typename T, typename OperationError>
struct list_iterator
{
    typedef std::ptrdiff_t                    difference_type;
    typedef std::bidirectional_iterator_tag   iterator_category;
    typedef T                                 value_type;
    typedef T*                                pointer;
    typedef T&                                reference;

    typedef list_node<T>                      node_t;

    list_iterator()
        : m_node()
    {
    }

    explicit list_iterator(list_node_base* node)
        : m_node(node)
    {
        NESTL_CHECK_LIST_NODE(m_node);
    }

    reference operator*() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);

        return static_cast<node_t*>(m_node)->get_reference();
    }

    pointer operator->() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);

        return static_cast<node_t*>(m_node)->get_pointer();
    }

    list_iterator& operator++() NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_next;
        NESTL_CHECK_LIST_NODE(m_node);

        return *this;
    }

    list_iterator operator++(int) NESTL_NOEXCEPT_SPEC
    {
        list_iterator res = *this;

        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_next;
        NESTL_CHECK_LIST_NODE(m_node);

        return res;
    }

    list_iterator& operator--() NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_prev;
        NESTL_CHECK_LIST_NODE(m_node);

        return *this;
    }

    list_iterator operator--(int) NESTL_NOEXCEPT_SPEC
    {
        list_iterator res = *this;

        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_prev;
        NESTL_CHECK_LIST_NODE(m_node);

        return res;
    }

    bool operator==(const list_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        NESTL_CHECK_LIST_NODE(other.m_node);

        return m_node == other.m_node;
    }

    bool operator!=(const list_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        NESTL_CHECK_LIST_NODE(other.m_node);

        return m_node != other.m_node;
    }

    list_node_base* get_list_node() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);

        return m_node;
    }

private:

    list_node_base* m_node;
};


template <typename T, typename OperationError>
struct list_const_iterator
{
    typedef std::ptrdiff_t                    difference_type;
    typedef std::bidirectional_iterator_tag   iterator_category;
    typedef T                                 value_type;
    typedef const T*                          pointer;
    typedef const T&                          reference;
    typedef OperationError                    operation_error;

    typedef list_iterator<T, operation_error> iterator;

    typedef const list_node<T>                node_t;

    list_const_iterator()
        : m_node()
    {
    }

    explicit list_const_iterator(const list_node_base* node) NESTL_NOEXCEPT_SPEC
        : m_node(node)
    {
        NESTL_CHECK_LIST_NODE(m_node);
    }

    list_const_iterator(const iterator& i) NESTL_NOEXCEPT_SPEC
        : m_node(i.get_list_node())
    {
        NESTL_CHECK_LIST_NODE(m_node);
    }


    reference operator*() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        return static_cast<node_t*>(m_node)->get_reference();
    }

    pointer operator->() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        return static_cast<node_t*>(m_node)->get_pointer();
    }

    list_const_iterator& operator++() NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_next;
        NESTL_CHECK_LIST_NODE(m_node);

        return *this;
    }

    list_const_iterator operator++(int) NESTL_NOEXCEPT_SPEC
    {
        list_const_iterator res = *this;

        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_next;
        NESTL_CHECK_LIST_NODE(m_node);

        return res;
    }

    list_const_iterator& operator--() NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_prev;
        NESTL_CHECK_LIST_NODE(m_node);

        return *this;
    }

    list_const_iterator operator--(int) NESTL_NOEXCEPT_SPEC
    {
        list_const_iterator res = *this;

        NESTL_CHECK_LIST_NODE(m_node);
        m_node = m_node->m_prev;
        NESTL_CHECK_LIST_NODE(m_node);

        return res;
    }

    bool operator==(const list_const_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        NESTL_CHECK_LIST_NODE(other.m_node);

        return m_node == other.m_node;
    }

    bool operator!=(const list_const_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);
        NESTL_CHECK_LIST_NODE(other.m_node);

        return m_node != other.m_node;
    }


    list_node_base* get_list_node() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_CHECK_LIST_NODE(m_node);

        return const_cast<list_node_base*>(m_node);
    }

private:

    const list_node_base* m_node;
};

} // namespace detail

template <typename T, typename Allocator = nestl::allocator<T> >
class list : private nestl::noncopyable
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
    typedef nestl::error_condition                                          operation_error;

    typedef detail::list_iterator<value_type, operation_error>              iterator;
    typedef detail::list_const_iterator<value_type, operation_error>        const_iterator;
    typedef std::reverse_iterator<iterator>                                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                           const_reverse_iterator;


    typedef nestl::result_with_operation_error<iterator, operation_error>   iterator_with_operation_error;

    // constructors
    explicit list(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC;

    explicit list(list&& other) NESTL_NOEXCEPT_SPEC;

    // destructor
    ~list() NESTL_NOEXCEPT_SPEC;

    // allocator support
    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC;

    // assignment operators and functions
    list& operator=(list&& other) NESTL_NOEXCEPT_SPEC;

    operation_error assign_copy(const list& other) NESTL_NOEXCEPT_SPEC;

    operation_error assign(size_type n, const_reference val = value_type()) NESTL_NOEXCEPT_SPEC;

    template <typename InputIterator>
    operation_error assign(InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    // element access
    reference front() NESTL_NOEXCEPT_SPEC;

    const_reference front() const NESTL_NOEXCEPT_SPEC;

    reference back() NESTL_NOEXCEPT_SPEC;

    const_reference back() const NESTL_NOEXCEPT_SPEC;

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



    // modifiers
    void clear() NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert(const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert(const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC;

    operation_error insert(const_iterator pos, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template<typename InputIterator>
    operation_error insert(const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    template<typename ... Args>
    iterator_with_operation_error emplace(const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC;

    iterator erase(const_iterator pos) NESTL_NOEXCEPT_SPEC;

    iterator erase(const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    operation_error push_back(const value_type& value) NESTL_NOEXCEPT_SPEC;

    operation_error push_back(value_type&& value) NESTL_NOEXCEPT_SPEC;

    template<typename ... Args>
    operation_error emplace_back(Args&& ... args) NESTL_NOEXCEPT_SPEC;

    void pop_back() NESTL_NOEXCEPT_SPEC;

    operation_error push_front(const value_type& value) NESTL_NOEXCEPT_SPEC;

    operation_error push_front(value_type&& value) NESTL_NOEXCEPT_SPEC;

    template<typename ... Args>
    operation_error emplace_front(Args&& ... args) NESTL_NOEXCEPT_SPEC;

    void pop_front() NESTL_NOEXCEPT_SPEC;

    operation_error resize(size_type count) NESTL_NOEXCEPT_SPEC;

    operation_error resize(size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

    void swap(list& other) NESTL_NOEXCEPT_SPEC;

    // operations
    void merge(list& other) NESTL_NOEXCEPT_SPEC;

    void merge(list&& other) NESTL_NOEXCEPT_SPEC;

    template <typename Compare>
    void merge(list& other, Compare comp) NESTL_NOEXCEPT_SPEC;

    template <typename Compare>
    void merge(list&& other, Compare comp) NESTL_NOEXCEPT_SPEC;

    void splice(const_iterator pos, list& other) NESTL_NOEXCEPT_SPEC;

    void splice(const_iterator pos, list&& other) NESTL_NOEXCEPT_SPEC;

    void splice(const_iterator pos, list& other, const_iterator it) NESTL_NOEXCEPT_SPEC;

    void splice(const_iterator pos, list&& other, const_iterator it) NESTL_NOEXCEPT_SPEC;

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    void remove(const value_type& value) NESTL_NOEXCEPT_SPEC;

    template <typename UnaryPredicate>
    void remove(UnaryPredicate p) NESTL_NOEXCEPT_SPEC;

    void reverse() NESTL_NOEXCEPT_SPEC;

    void unique() NESTL_NOEXCEPT_SPEC;

    template<typename BinaryPredicate>
    void unique(BinaryPredicate p) NESTL_NOEXCEPT_SPEC;

    void sort() NESTL_NOEXCEPT_SPEC;

    template<typename Compare>
    void sort(Compare comp) NESTL_NOEXCEPT_SPEC;


private:

    typedef detail::list_node<value_type> node_type;
    typedef typename allocator_type::template rebind<node_type>::other node_allocator_type;

    node_allocator_type m_node_allocator;

    detail::list_node_base m_node;

    void init_empty_list() NESTL_NOEXCEPT_SPEC;

    void destroy_list_content() NESTL_NOEXCEPT_SPEC;

    void swap_data(list& other) NESTL_NOEXCEPT_SPEC;

    void move_assign(const std::true_type& /* true_val */, list&& other) NESTL_NOEXCEPT_SPEC;

    template <typename ... Args>
    operation_error create_node(node_type*& node, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    template <typename ListIterator1, typename ListIterator2, typename ListIterator3>
    void transfer(ListIterator1 pos, ListIterator2 first, ListIterator3 last) NESTL_NOEXCEPT_SPEC;
};



/**
 * @brief specialization of class_traits for nestl::list
 *
 * Allows construct one list from another (emulate copy construction)
 */
template <typename T, typename ListAllocator>
struct class_traits <nestl::list<T, ListAllocator> >
{
    typedef nestl::list<T, ListAllocator> list_t;

    template <typename OperationError, typename Allocator>
    static OperationError construct(list_t* ptr, Allocator& alloc) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);

        return OperationError();
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(list_t* ptr, Allocator& alloc, list_t&& other) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr, std::move(other));

        return OperationError();
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(list_t* ptr, Allocator& alloc, const list_t& other) NESTL_NOEXCEPT_SPEC
    {
        alloc.construct(ptr);

        list_t* end = ptr + 1;
        nestl::detail::destruction_scoped_guard<list_t*, Allocator> guard(ptr, end, alloc);

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
bool operator == (const nestl::list<T, Allocator>& left, const nestl::list<T, Allocator>& right) NESTL_NOEXCEPT_SPEC
{
    if (left.size() != right.size())
    {
        return false;
    }

    typedef typename nestl::list<T, Allocator>::const_iterator iterator_t;
    iterator_t leftIt = left.cbegin();
    iterator_t leftEnd = left.cend();

    iterator_t rightIt = right.cbegin();


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
list<T, A>::list(const allocator_type& alloc) NESTL_NOEXCEPT_SPEC
    : m_node_allocator(alloc)
    , m_node()
{
    init_empty_list();
}

template <typename T, typename A>
list<T, A>::list(list&& other) NESTL_NOEXCEPT_SPEC
    : m_node_allocator(std::move(other.m_node_allocator))
    , m_node()
{
    init_empty_list();
    swap_data(other);
}

template <typename T, typename A>
list<T, A>::~list() NESTL_NOEXCEPT_SPEC
{
    destroy_list_content();
}

template <typename T, typename A>
typename list<T, A>::allocator_type list<T, A>::get_allocator() const NESTL_NOEXCEPT_SPEC
{
    return allocator_type(m_node_allocator);
}

template <typename T, typename A>
list<T, A>& list<T, A>::operator=(list&& other) NESTL_NOEXCEPT_SPEC
{
    this->move_assign(typename nestl::allocator_traits<node_allocator_type>::propagate_on_container_move_assignment(), other);

    return *this;
}

template <typename T, typename A>
typename list<T, A>::operation_error list<T, A>::assign_copy(const list& other) NESTL_NOEXCEPT_SPEC
{
    return this->assign(other.cbegin(), other.cend());
}

template <typename T, typename A>
typename list<T, A>::operation_error list<T, A>::assign(size_type n, const_reference val) NESTL_NOEXCEPT_SPEC
{
    list tmp; tmp.swap(*this);

    while (n > 0)
    {
        operation_error err = this->push_back(val);
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
typename list<T, A>::operation_error
list<T, A>::assign(InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    list tmp; tmp.swap(*this);

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

template <typename T, typename A>
typename list<T, A>::reference
list<T, A>::front() NESTL_NOEXCEPT_SPEC
{
	assert(!empty());
    return *begin();
}

template <typename T, typename A>
typename list<T, A>::const_reference
list<T, A>::front() const NESTL_NOEXCEPT_SPEC
{
	assert(!empty());
    return *begin();
}

template <typename T, typename A>
typename list<T, A>::reference
list<T, A>::back() NESTL_NOEXCEPT_SPEC
{
	assert(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename list<T, A>::const_reference
list<T, A>::back() const NESTL_NOEXCEPT_SPEC
{
	assert(!empty());
    return *rbegin();
}


template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::begin() NESTL_NOEXCEPT_SPEC
{
    return iterator(m_node.m_next);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::begin() const NESTL_NOEXCEPT_SPEC
{
    return const_iterator(m_node.m_next);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::cbegin() const NESTL_NOEXCEPT_SPEC
{
    return const_iterator(m_node.m_next);
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::end() NESTL_NOEXCEPT_SPEC
{
    return iterator(&m_node);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::end() const NESTL_NOEXCEPT_SPEC
{
    return const_iterator(&m_node);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::cend() const NESTL_NOEXCEPT_SPEC
{
    return const_iterator(&m_node);
}

template <typename T, typename A>
typename list<T, A>::reverse_iterator
list<T, A>::rbegin() NESTL_NOEXCEPT_SPEC
{
    return reverse_iterator(end());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::rbegin() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(end());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::crbegin() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(cend());
}

template <typename T, typename A>
typename list<T, A>::reverse_iterator
list<T, A>::rend() NESTL_NOEXCEPT_SPEC
{
    return reverse_iterator(begin());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::rend() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(begin());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::crend() const NESTL_NOEXCEPT_SPEC
{
    return const_reverse_iterator(cbegin());
}


template <typename T, typename A>
bool
list<T, A>::empty() const NESTL_NOEXCEPT_SPEC
{
    return m_node.m_next == &m_node;
}

template <typename T, typename A>
typename list<T, A>::size_type
list<T, A>::size() const NESTL_NOEXCEPT_SPEC
{
    return nestl::distance(cbegin(), cend());
}

template <typename T, typename A>
typename list<T, A>::size_type
list<T, A>::max_size() const NESTL_NOEXCEPT_SPEC
{
    return m_node_allocator.max_size();
}


template <typename T, typename A>
void
list<T, A>::clear() NESTL_NOEXCEPT_SPEC
{
    destroy_list_content();
    init_empty_list();
}

template <typename T, typename A>
typename list<T, A>::iterator_with_operation_error
list<T, A>::insert(const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return emplace(pos, value);
}

template <typename T, typename A>
typename list<T, A>::iterator_with_operation_error
list<T, A>::insert(const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	return emplace(pos, std::move(value));
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::insert(const_iterator pos, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    while (count > 0)
    {
        iterator_with_operation_error err = insert(pos, value);
        if (err)
        {
            return err.error();
        }

        pos = err.result();
        --count;
    }

    return operation_error();
}

template <typename T, typename A>
template<typename InputIterator>
typename list<T, A>::operation_error
list<T, A>::insert(const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    while (first != last)
    {
        iterator_with_operation_error err = insert(pos, *first);
        if (err)
        {
            return err.error();
        }

        pos = err.result();
        ++first;
    }

    return operation_error();
}

template <typename T, typename A>
template<typename ... Args>
typename list<T, A>::iterator_with_operation_error
list<T, A>::emplace(const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC
{
    node_type* newNode = 0;
	auto err = create_node(newNode, std::forward<Args>(args)...);
    if (err)
    {
        return nestl::make_result_with_operation_error(end(), err);
    }
    newNode->init_empty();
    NESTL_CHECK_LIST_NODE(newNode);

    newNode->inject(pos.get_list_node());

    return nestl::make_result_with_operation_error(iterator(newNode), err);
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::erase(const_iterator pos) NESTL_NOEXCEPT_SPEC
{
    node_type* node = static_cast<node_type*>(pos.get_list_node());
    iterator ret = iterator(node->m_next);

    node->remove();

    allocator_type alloc = get_allocator();
    node->destroy_value(alloc);
    m_node_allocator.deallocate(node, 1);

    return ret;
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::erase(const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC
{
    while (first != last)
    {
        first = erase(first);
    }

    return last.get_list_node();
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_back(const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return insert(cend(), value).error();
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_back(value_type&& value) NESTL_NOEXCEPT_SPEC
{
	return insert(cend(), std::move(value)).error();
}

template <typename T, typename A>
template<typename ... Args>
typename list<T, A>::operation_error
list<T, A>::emplace_back(Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	return emplace(cend(), std::forward<Args>(args) ...).error();
}

template <typename T, typename A>
void
list<T, A>::pop_back() NESTL_NOEXCEPT_SPEC
{
    erase(iterator(m_node.m_prev));
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_front(const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return insert(cbegin(), value).error();
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_front(value_type&& value) NESTL_NOEXCEPT_SPEC
{
    return insert(cbegin(), std::move(value)).error();
}

template <typename T, typename A>
template<typename ... Args>
typename list<T, A>::operation_error
list<T, A>::emplace_front(Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	return emplace(cbegin(), std::forward<Args>(args) ...).error();
}

template <typename T, typename A>
void
list<T, A>::pop_front() NESTL_NOEXCEPT_SPEC
{
    erase(cbegin());
}


template <typename T, typename A>
void
list<T, A>::swap(list& other) NESTL_NOEXCEPT_SPEC
{
    swap_data(other);
}

/// Operations
template <typename T, typename A>
void
list<T, A>::merge(list& other) NESTL_NOEXCEPT_SPEC
{
	merge(std::move(other));
}

template <typename T, typename A>
void
list<T, A>::merge(list&& other) NESTL_NOEXCEPT_SPEC
{
	merge(std::forward<list&&>(other), std::less<T>());
}

template <typename T, typename A>
template <typename Compare>
void
list<T, A>::merge(list& other, Compare comp) NESTL_NOEXCEPT_SPEC
{
	merge(std::move(other), comp);
}

template <typename T, typename A>
template <typename Compare>
void
list<T, A>::merge(list&& other, Compare comp) NESTL_NOEXCEPT_SPEC
{
    assert(this != &other);

    iterator first1 = begin();
    iterator last1 = end();

    iterator first2 = other.begin();
    iterator last2 = other.end();

    while ((first1 != last1) && (first2 != last2))
    {
        if (comp(*first2, *first1))
        {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        }
        else
        {
            ++first1;
        }
    }

    if (first2 != last2)
    {
        transfer(last1, first2, last2);
    }
}

template <typename T, typename A>
void
list<T, A>::splice(const_iterator pos, list& other) NESTL_NOEXCEPT_SPEC
{
    splice(pos, std::move(other));
}

template <typename T, typename A>
void
list<T, A>::splice(const_iterator pos, list&& other) NESTL_NOEXCEPT_SPEC
{
    if (!other.empty())
    {
        this->transfer(pos, other.begin(), other.end());
    }
}

template <typename T, typename A>
void
list<T, A>::splice(const_iterator pos, list& other, const_iterator it) NESTL_NOEXCEPT_SPEC
{
    splice(pos, std::move(other), it);
}

template <typename T, typename A>
void
list<T, A>::splice(const_iterator pos, list&& /* other */, const_iterator it) NESTL_NOEXCEPT_SPEC
{
    iterator j = iterator(it.get_list_node());
    ++j;
    if ((pos == it) || (pos == j))
    {
        return;
    }

    this->transfer(pos, it, j);
}

template <typename T, typename A>
void
list<T, A>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC
{
    splice(pos, std::move(other), first, last);
}

template <typename T, typename A>
void
list<T, A>::splice(const_iterator pos, list&& /* other */, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC
{
    if (first != last)
    {
        this->transfer(pos, first, last);
    }
}



template <typename T, typename A>
void
list<T, A>::sort() NESTL_NOEXCEPT_SPEC
{
    sort(std::less<value_type>());
}

template <typename T, typename A>
template<typename Compare>
void
list<T, A>::sort(Compare comp) NESTL_NOEXCEPT_SPEC
{
    // Do nothing if the list has length 0 or 1.
    if ((this->m_node.m_next != &this->m_node) &&
        (this->m_node.m_next->m_next != &this->m_node))
    {
        list carry;
        list tmp[64];
        list* fill = &tmp[0];
        list* counter;

        do
        {
            carry.splice(carry.begin(), *this, begin());

            for(counter = &tmp[0]; counter != fill && !counter->empty(); ++counter)
            {
                counter->merge(carry, comp);
                carry.swap(*counter);
            }

            carry.swap(*counter);
            if (counter == fill)
            {
                ++fill;
            }
        }
        while (!empty());

        for (counter = &tmp[1]; counter != fill; ++counter)
        {
            counter->merge(*(counter - 1), comp);
        }
        swap( *(fill - 1) );
    }
}



/// Private implementation


template <typename T, typename A>
void
list<T, A>::init_empty_list() NESTL_NOEXCEPT_SPEC
{
    m_node.init_empty();
}

template <typename T, typename A>
void
list<T, A>::destroy_list_content() NESTL_NOEXCEPT_SPEC
{
    node_type* current = static_cast<node_type*>(m_node.m_next);
    allocator_type alloc = get_allocator();

    while (current != &m_node)
    {
        node_type* tmp = current;
        current = static_cast<node_type*>(current->m_next);

        tmp->destroy_value(alloc);
        m_node_allocator.deallocate(tmp, 1);
    }
}

template <typename T, typename A>
void
list<T, A>::swap_data(list& other) NESTL_NOEXCEPT_SPEC
{
    nestl::detail::list_node_base::swap(other.m_node, m_node);
}

template <typename T, typename A>
void
list<T, A>::move_assign(const std::true_type& /* true_val */, list&& other) NESTL_NOEXCEPT_SPEC
{
    const list tmp(std::move(*this));
    this->swap_data(other);
}

template <typename T, typename A>
template <typename ... Args>
typename list<T, A>::operation_error
list<T, A>::create_node(node_type*& node, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    node_type* tmp = m_node_allocator.allocate(1);
    if (!tmp)
    {
        return operation_error(nestl::errc::not_enough_memory);
    }
    nestl::detail::allocation_scoped_guard<node_type*, node_allocator_type> allocGuard(m_node_allocator, tmp, 1);

    allocator_type alloc = get_allocator();

	auto err = tmp-> template initialize<operation_error>(alloc, std::forward<Args>(args) ...);
    if (err)
    {
        return err;
    }

    allocGuard.release();
    node = tmp;

    return err;
}

template <typename T, typename A>
template <typename ListIterator1, typename ListIterator2, typename ListIterator3>
void
list<T, A>::transfer(ListIterator1 pos, ListIterator2 first, ListIterator3 last) NESTL_NOEXCEPT_SPEC
{
    pos.get_list_node()->transfer(first.get_list_node(), last.get_list_node());
}

} // namespace nestl

#endif /* NESTL_LIST_HPP */
