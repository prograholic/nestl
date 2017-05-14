/**
 * @file list.hpp - implementation of nestl::list container
 *
 * @note Implementation based on implementation of std::list from libstdc++
 */

#ifndef NESTL_IMPLEMENTATION_LIST_HPP
#define NESTL_IMPLEMENTATION_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/class_operations.hpp>
#include <nestl/algorithm.hpp>
#include <nestl/alignment.hpp>

#include <nestl/detail/destroy.hpp>

#include <cassert>

namespace nestl
{
namespace impl
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

    void destroy_value()
    {
        T* value = get_pointer();
        nestl::detail::destroy(value, value + 1);
    }

    template <typename OperationError, typename ... Args>
    void initialize(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
    {
		nestl::class_operations::construct(err, get_pointer(), std::forward<Args>(args) ...);
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


template <typename T>
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


template <typename T>
struct list_const_iterator
{
    typedef std::ptrdiff_t                    difference_type;
    typedef std::bidirectional_iterator_tag   iterator_category;
    typedef T                                 value_type;
    typedef const T*                          pointer;
    typedef const T&                          reference;

    typedef list_iterator<T>                  iterator;

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

template <typename T, typename Allocator = nestl::allocator<T> >
class list
{
    list(const list&) = delete;
    list& operator=(const list&) = delete;
public:
    typedef T                                                               value_type;
    typedef Allocator                                                       allocator_type;
    typedef std::size_t                                                     size_type;
	typedef std::ptrdiff_t                                                  difference_type;
    typedef T&                                                              reference;
    typedef const T&                                                        const_reference;
    typedef typename nestl::allocator_traits<allocator_type>::pointer       pointer;
    typedef typename nestl::allocator_traits<allocator_type>::const_pointer const_pointer;
    typedef list_iterator<value_type>                                       iterator;
    typedef list_const_iterator<value_type>                                 const_iterator;
    typedef std::reverse_iterator<iterator>                                 reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                           const_reverse_iterator;

    // constructors
    explicit list(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC;

    explicit list(list&& other) NESTL_NOEXCEPT_SPEC;

    // destructor
    ~list() NESTL_NOEXCEPT_SPEC;

    // allocator support
    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC;

    // assignment operators and functions
    list& operator=(list&& other) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void copy_nothrow(OperationError& err, const list& other) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void assign_nothrow(OperationError& err, size_type n, const_reference val = value_type()) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename InputIterator>
    void assign_nothrow(OperationError& err, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

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

    iterator erase(const_iterator pos) NESTL_NOEXCEPT_SPEC;

    iterator erase(const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void push_back_nothrow(OperationError& err, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void push_back_nothrow(OperationError& err, value_type&& value) NESTL_NOEXCEPT_SPEC;

    template<typename OperationError, typename ... Args>
    void emplace_back_nothrow(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    void pop_back() NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void push_front_nothrow(OperationError& err, const value_type& value) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void push_front_nothrow(OperationError& err, value_type&& value) NESTL_NOEXCEPT_SPEC;

    template<typename OperationError, typename ... Args>
    void emplace_front_nothrow(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    void pop_front() NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void resize_nothrow(OperationError& err, size_type count) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError>
    void resize_nothrow(OperationError& err, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC;

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

    typedef list_node<value_type> node_type;
    typedef typename allocator_type::template rebind<node_type>::other node_allocator_type;

    node_allocator_type m_node_allocator;

    list_node_base m_node;

    void init_empty_list() NESTL_NOEXCEPT_SPEC;

    void destroy_list_content() NESTL_NOEXCEPT_SPEC;

    void swap_data(list& other) NESTL_NOEXCEPT_SPEC;

    void move_assign(const std::true_type& /* true_val */, list&& other) NESTL_NOEXCEPT_SPEC;

    template <typename OperationError, typename ... Args>
    node_type* create_node(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC;

    template <typename ListIterator1, typename ListIterator2, typename ListIterator3>
    void transfer(ListIterator1 pos, ListIterator2 first, ListIterator3 last) NESTL_NOEXCEPT_SPEC;
};

} // namespace impl

// Added emulation of copy construction
template <typename T, typename Allocator>
struct two_phase_initializator<impl::list<T, Allocator>>
{
    template <typename OperationError>
    static void init(OperationError& err,
                     impl::list<T, Allocator>& defaultConstructed,
                     const impl::list<T, Allocator>& other) NESTL_NOEXCEPT_SPEC
    {
        defaultConstructed.copy_nothrow(err, other);
    }
};

namespace impl
{

template <typename T, typename Allocator>
bool operator == (const list<T, Allocator>& left, const list<T, Allocator>& right) NESTL_NOEXCEPT_SPEC
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
template <typename OperationError>
void
list<T, A>::copy_nothrow(OperationError& err, const list& other) NESTL_NOEXCEPT_SPEC
{
    assign_nothrow(err, other.cbegin(), other.cend());
}

template <typename T, typename A>
template <typename OperationError>
void
list<T, A>::assign_nothrow(OperationError& err, size_type n, const_reference val) NESTL_NOEXCEPT_SPEC
{
    list tmp; tmp.swap(*this);

    while (n > 0)
    {
        push_back_nothrow(err, val);
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
list<T, A>::assign_nothrow(OperationError& err, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    list tmp; tmp.swap(*this);

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
template <typename OperationError>
typename list<T, A>::iterator
list<T, A>::insert_nothrow(OperationError& err, const_iterator pos, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    return emplace_nothrow(err, pos, value);
}

template <typename T, typename A>
template <typename OperationError>
typename list<T, A>::iterator
list<T, A>::insert_nothrow(OperationError& err, const_iterator pos, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	return emplace_nothrow(err, pos, std::move(value));
}

template <typename T, typename A>
template <typename OperationError>
void
list<T, A>::insert_nothrow(OperationError& err, const_iterator pos, size_type count, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    while (count > 0)
    {
        pos = insert_nothrow(err, pos, value);
        if (err)
        {
            return;
        }

        --count;
    }
}

template <typename T, typename A>
template<typename OperationError, typename InputIterator>
void
list<T, A>::insert_nothrow(OperationError& err, const_iterator pos, InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC
{
    while (first != last)
    {
        pos = insert_nothrow(err, pos, *first);
        if (err)
        {
            return;
        }

        ++first;
    }
}

template <typename T, typename A>
template<typename OperationError, typename ... Args>
typename list<T, A>::iterator
list<T, A>::emplace_nothrow(OperationError& err, const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC
{
    node_type* newNode = create_node(err, std::forward<Args>(args)...);
    if (err)
    {
        return end();
    }

    newNode->init_empty();
    NESTL_CHECK_LIST_NODE(newNode);

    newNode->inject(pos.get_list_node());

    return iterator(newNode);
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::erase(const_iterator pos) NESTL_NOEXCEPT_SPEC
{
    node_type* node = static_cast<node_type*>(pos.get_list_node());
    iterator ret = iterator(node->m_next);

    node->remove();

    node->destroy_value();
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
template <typename OperationError>
void
list<T, A>::push_back_nothrow(OperationError& err, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    insert_nothrow(err, cend(), value);
}

template <typename T, typename A>
template <typename OperationError>
void
list<T, A>::push_back_nothrow(OperationError& err, value_type&& value) NESTL_NOEXCEPT_SPEC
{
	insert_nothrow(err, cend(), std::move(value));
}

template <typename T, typename A>
template<typename OperationError, typename ... Args>
void
list<T, A>::emplace_back_nothrow(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	emplace_nothrow(err, cend(), std::forward<Args>(args) ...);
}

template <typename T, typename A>
void
list<T, A>::pop_back() NESTL_NOEXCEPT_SPEC
{
    erase(iterator(m_node.m_prev));
}

template <typename T, typename A>
template <typename OperationError>
void
list<T, A>::push_front_nothrow(OperationError& err, const value_type& value) NESTL_NOEXCEPT_SPEC
{
    insert_nothrow(err, cbegin(), value);
}

template <typename T, typename A>
template <typename OperationError>
void
list<T, A>::push_front_nothrow(OperationError& err, value_type&& value) NESTL_NOEXCEPT_SPEC
{
    insert_nothrow(err, cbegin(), std::move(value));
}

template <typename T, typename A>
template<typename OperationError, typename ... Args>
void
list<T, A>::emplace_front_nothrow(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
	emplace_nothrow(err, cbegin(), std::forward<Args>(args) ...);
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

    while (current != &m_node)
    {
        node_type* tmp = current;
        current = static_cast<node_type*>(current->m_next);

        tmp->destroy_value();
        m_node_allocator.deallocate(tmp, 1);
    }
}

template <typename T, typename A>
void
list<T, A>::swap_data(list& other) NESTL_NOEXCEPT_SPEC
{
    list_node_base::swap(other.m_node, m_node);
}

template <typename T, typename A>
void
list<T, A>::move_assign(const std::true_type& /* true_val */, list&& other) NESTL_NOEXCEPT_SPEC
{
    const list tmp(std::move(*this));
    this->swap_data(other);
}

template <typename T, typename A>
template <typename OperationError, typename ... Args>
typename list<T, A>::node_type*
list<T, A>::create_node(OperationError& err, Args&& ... args) NESTL_NOEXCEPT_SPEC
{
    node_type* node = allocator_traits<node_allocator_type>::allocate(err, m_node_allocator, 1);
    if (err)
    {
        return nullptr;
    }
    nestl::detail::deallocation_scoped_guard<node_type*, node_allocator_type> allocGuard(m_node_allocator, node, 1);

	node->initialize(err, std::forward<Args>(args) ...);
    if (err)
    {
        return nullptr;
    }

    allocGuard.release();
    return node;
}

template <typename T, typename A>
template <typename ListIterator1, typename ListIterator2, typename ListIterator3>
void
list<T, A>::transfer(ListIterator1 pos, ListIterator2 first, ListIterator3 last) NESTL_NOEXCEPT_SPEC
{
    pos.get_list_node()->transfer(first.get_list_node(), last.get_list_node());
}

} // namespace impl
} // namespace nestl

#endif /* NESTL_IMPLEMENTATION_LIST_HPP */
