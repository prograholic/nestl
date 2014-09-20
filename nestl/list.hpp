#ifndef NESTL_LIST_HPP
#define NESTL_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/allocator.hpp>
#include <nestl/memory.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/operation_error.hpp>


#include <system_error>
#include <initializer_list>
#include <iterator>

#include <list>

namespace nestl
{


namespace detail
{

struct list_node_base
{
    list_node_base* m_next;
    list_node_base* m_prev;

    static void swap(list_node_base& left, list_node_base& right)
    {
        std::swap(left.m_prev, right.m_prev);
        std::swap(left.m_next, right.m_next);
    }


    void inject(list_node_base* node)
    {
        NESTL_ASSERT(node);

        list_node_base* next = node->m_next;
        NESTL_ASSERT(next->m_prev == node);

        node->m_next = this;
        next->m_prev = this;

        m_next = next;
        m_prev = node;
    }

    void remove()
    {
        NESTL_ASSERT(0 && "not implemented");
    }
};


template <typename T>
struct list_node : public list_node_base
{
    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_value;

    list_node() noexcept
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
    OperationError initialize(Allocator& a, Args&& ... args) noexcept
    {
        auto err = nestl::detail::construct<OperationError>(get_pointer(), a, std::forward<Args>(args) ...);
        return err;
    }


    T* get_pointer() noexcept
    {
        return reinterpret_cast<T*>(&m_value);
    }

    T& get_reference() noexcept
    {
        return *get_pointer();
    }


    const T* get_pointer() const noexcept
    {
        return reinterpret_cast<const T*>(&m_value);
    }

    const T& get_reference() const noexcept
    {
        return *get_pointer();
    }
};


template <typename T>
struct list_iterator
{
    typedef std::ptrdiff_t                  difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef T&                              reference;

    typedef list_node<T>                    node_t;

    list_iterator()
        : m_node()
    {
    }

    explicit list_iterator(list_node_base* node)
        : m_node(node)
    {
    }

    reference operator*() const noexcept
    {
        NESTL_ASSERT(m_node);
        return static_cast<node_t*>(m_node)->get_reference();
    }

    pointer operator->() const noexcept
    {
        NESTL_ASSERT(m_node);
        return static_cast<node_t*>(m_node)->get_pointer();
    }

    list_iterator& operator++() noexcept
    {
        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_next);
        m_node = m_node->m_next;

        return *this;
    }

    list_iterator operator++(int) noexcept
    {
        list_iterator res = *this;

        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_next);

        m_node = m_node->m_next;

        return res;
    }

    list_iterator& operator--() noexcept
    {
        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_prev);
        m_node = m_node->m_prev;

        return *this;
    }

    list_iterator operator--(int) noexcept
    {
        list_iterator res = *this;

        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_prev);

        m_node = m_node->m_prev;

        return res;
    }

    bool operator==(const list_iterator& other) const noexcept
    {
        return m_node == other.m_node;
    }

    bool operator!=(const list_iterator& other) const noexcept
    {
        return m_node != other.m_node;
    }

    list_node_base* get_list_node() const
    {
        return m_node;
    }

private:

    list_node_base* m_node;
};


template <typename T>
struct list_const_iterator
{
    typedef std::ptrdiff_t                  difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef const T*                        pointer;
    typedef const T&                        reference;

    typedef list_iterator<T>                iterator;

    typedef const list_node<T>              node_t;

    list_const_iterator()
        : m_node()
    {
    }

    explicit list_const_iterator(const list_node_base* node)
        : m_node(node)
    {
    }

    list_const_iterator(const iterator& i) noexcept
        : m_node(i.get_list_node())
    {
    }


    reference operator*() const noexcept
    {
        NESTL_ASSERT(m_node);
        return static_cast<node_t*>(m_node)->get_reference();
    }

    pointer operator->() const noexcept
    {
        NESTL_ASSERT(m_node);
        return static_cast<node_t*>(m_node)->get_pointer();
    }

    list_const_iterator& operator++() noexcept
    {
        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_next);
        m_node = m_node->m_next;

        return *this;
    }

    list_const_iterator operator++(int) noexcept
    {
        list_const_iterator res = *this;

        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_next);

        m_node = m_node->m_next;

        return res;
    }

    list_const_iterator& operator--() noexcept
    {
        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_prev);
        m_node = m_node->m_prev;

        return *this;
    }

    list_const_iterator operator--(int) noexcept
    {
        list_const_iterator res = *this;

        NESTL_ASSERT(m_node);
        NESTL_ASSERT(m_node->m_prev);

        m_node = m_node->m_prev;

        return res;
    }

    bool operator==(const list_const_iterator& other) const noexcept
    {
        return m_node == other.m_node;
    }

    bool operator!=(const list_const_iterator& other) const noexcept
    {
        return m_node != other.m_node;
    }


    list_node_base* get_list_node() const
    {
        return const_cast<list_node_base*>(m_node);
    }

private:

    const list_node_base* m_node;
};

} // namespace detail

template <typename T, typename Allocator = nestl::allocator<T>>
class list : private nestl::noncopyable
{
public:
    typedef T                                                             value_type;
    typedef Allocator                                                     allocator_type;
    typedef std::size_t                                                   size_type;
    typedef std::ptrdiff_t                                                difference_type;
    typedef T&                                                            reference;
    typedef const T&                                                      const_reference;
    typedef typename std::allocator_traits<allocator_type>::pointer       pointer;
    typedef typename std::allocator_traits<allocator_type>::const_pointer const_pointer;

    typedef detail::list_iterator<value_type>                             iterator;
    typedef detail::list_const_iterator<value_type>                       const_iterator;
    typedef std::reverse_iterator<iterator>                               reverse_iterator;
    typedef std::reverse_iterator<const_iterator>                         const_reverse_iterator;

    typedef std::error_condition                                          operation_error;
    typedef nestl::result_with_operation_error<iterator, operation_error> iterator_with_operation_error;

    // constructors
    explicit list(const allocator_type& alloc = allocator_type()) noexcept;

    explicit list(list&& other) noexcept;

    // destructor
    ~list() noexcept;

    // allocator support
    allocator_type get_allocator() const noexcept;

    // assignment operators and functions
    list& operator=(list&& other) noexcept;

    operation_error assign_copy(const list& other) noexcept;

    operation_error assign(size_type n, const_reference val = value_type()) noexcept;

    template <typename InputIterator>
    operation_error assign(InputIterator first, InputIterator last) noexcept;

    operation_error assign(std::initializer_list<value_type> ilist) noexcept;

    // element access
    reference front() noexcept;

    const_reference front() const noexcept;

    reference back() noexcept;

    const_reference back() const noexcept;

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



    // modifiers
    void clear() noexcept;

    iterator_with_operation_error insert(const_iterator pos, const value_type& value) noexcept;

    iterator_with_operation_error insert(const_iterator pos, value_type&& value) noexcept;

    operation_error insert(const_iterator pos, size_type count, const value_type& value) noexcept;

    template<typename InputIterator>
    operation_error insert(const_iterator pos, InputIterator first, InputIterator last) noexcept;

    iterator_with_operation_error insert(const_iterator pos, std::initializer_list<T> ilist) noexcept;

    template<typename ... Args>
    iterator_with_operation_error emplace(const_iterator pos, Args&&... args) noexcept;

    iterator erase(const_iterator pos) noexcept;

    iterator erase(const_iterator first, const_iterator last) noexcept;

    operation_error push_back(const value_type& value) noexcept;

    operation_error push_back(value_type&& value) noexcept;

    template<typename ... Args>
    operation_error emplace_back(Args&& ... args) noexcept;

    void pop_back() noexcept;

    operation_error push_front(const value_type& value) noexcept;

    operation_error push_front(value_type&& value) noexcept;

    template<typename ... Args>
    operation_error emplace_front(Args&& ... args) noexcept;

    void pop_front() noexcept;

    operation_error resize(size_type count) noexcept;

    operation_error resize(size_type count, const value_type& value) noexcept;

    void swap(list& other) noexcept;

    // operations
    void merge(list& other) noexcept;

    void merge(list&& other) noexcept;

    template <typename Compare>
    void merge(list& other, Compare comp) noexcept;

    template <typename Compare>
    void merge(list&& other, Compare comp) noexcept;

    void splice(const_iterator pos, list& other) noexcept;

    void splice(const_iterator pos, list&& other) noexcept;

    void splice(const_iterator pos, list& other, const_iterator it) noexcept;

    void splice(const_iterator pos, list&& other, const_iterator it) noexcept;

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) noexcept;

    void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last) noexcept;

    void remove(const value_type& value) noexcept;

    template <typename UnaryPredicate>
    void remove(UnaryPredicate p) noexcept;

    void reverse() noexcept;

    void unique() noexcept;

    template<typename BinaryPredicate>
    void unique(BinaryPredicate p);

    void sort();

    template<typename Compare>
    void sort(Compare comp);


private:

    typedef detail::list_node<value_type> node_type;
    typedef typename allocator_type::template rebind<node_type>::other node_allocator_type;

    node_allocator_type m_node_allocator;

    detail::list_node_base m_node;

    void init_empty_list() noexcept;

    void destroy_list_content() noexcept;

    void swap_data(list& other) noexcept;

    void move_assign(const std::true_type& /* true_val */, list&& other) noexcept;

    template <typename ... Args>
    operation_error create_node(node_type*& node, Args&& ... args) noexcept;
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
    static OperationError construct(list_t* ptr, Allocator& alloc) noexcept
    {
        alloc.construct(ptr);

        return OperationError();
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(list_t* ptr, Allocator& alloc, list_t&& other) noexcept
    {
        alloc.construct(ptr, std::move(other));

        return OperationError();
    }

    template <typename OperationError, typename Allocator>
    static OperationError construct(list_t* ptr, Allocator& alloc, const list_t& other) noexcept
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
bool operator == (const nestl::list<T, Allocator>& left, const nestl::list<T, Allocator>& right)
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
list<T, A>::list(const allocator_type& alloc) noexcept
    : m_node_allocator(alloc)
    , m_node()
{
    init_empty_list();
}

template <typename T, typename A>
list<T, A>::list(list&& other) noexcept
    : m_node_allocator(std::move(other.m_node_allocator))
    , m_node()
{
    init_empty_list();
    swap_data(other);
}

template <typename T, typename A>
list<T, A>::~list() noexcept
{
    destroy_list_content();
}

template <typename T, typename A>
typename list<T, A>::allocator_type list<T, A>::get_allocator() const noexcept
{
    return allocator_type(m_node_allocator);
}

template <typename T, typename A>
list<T, A>& list<T, A>::operator=(list&& other) noexcept
{
    this->move_assign(typename std::allocator_traits<node_allocator_type>::propagate_on_container_move_assignment(), other);

    return *this;
}

template <typename T, typename A>
typename list<T, A>::operation_error list<T, A>::assign_copy(const list& other) noexcept
{
    return this->assign(other.cbegin(), other.cend());
}

template <typename T, typename A>
typename list<T, A>::operation_error list<T, A>::assign(size_type n, const_reference val) noexcept
{
    const list tmp(std::move(*this));

    while (n > 0)
    {
        auto err = this->push_back(val);
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
list<T, A>::assign(InputIterator first, InputIterator last) noexcept
{
    const list tmp(std::move(*this));

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

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::assign(std::initializer_list<value_type> ilist) noexcept
{
    return this->assign(ilist.begin(), ilist.end());
}


template <typename T, typename A>
typename list<T, A>::reference
list<T, A>::front() noexcept
{
    NESTL_ASSERT(!empty());
    return *begin();
}

template <typename T, typename A>
typename list<T, A>::const_reference
list<T, A>::front() const noexcept
{
    NESTL_ASSERT(!empty());
    return *begin();
}

template <typename T, typename A>
typename list<T, A>::reference
list<T, A>::back() noexcept
{
    NESTL_ASSERT(!empty());
    return *rbegin();
}

template <typename T, typename A>
typename list<T, A>::const_reference
list<T, A>::back() const noexcept
{
    NESTL_ASSERT(!empty());
    return *rbegin();
}


template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::begin() noexcept
{
    return iterator(m_node.m_next);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::begin() const noexcept
{
    return const_iterator(m_node.m_next);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::cbegin() const noexcept
{
    return const_iterator(m_node.m_next);
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::end() noexcept
{
    return iterator(&m_node);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::end() const noexcept
{
    return const_iterator(&m_node);
}

template <typename T, typename A>
typename list<T, A>::const_iterator
list<T, A>::cend() const noexcept
{
    return const_iterator(&m_node);
}

template <typename T, typename A>
typename list<T, A>::reverse_iterator
list<T, A>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::crbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

template <typename T, typename A>
typename list<T, A>::reverse_iterator
list<T, A>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <typename T, typename A>
typename list<T, A>::const_reverse_iterator
list<T, A>::crend() const noexcept
{
    return const_reverse_iterator(cbegin());
}


template <typename T, typename A>
bool
list<T, A>::empty() const noexcept
{
    return m_node.m_next == &m_node;
}

template <typename T, typename A>
typename list<T, A>::size_type
list<T, A>::size() const noexcept
{
    return std::distance(cbegin(), cend());
}

template <typename T, typename A>
typename list<T, A>::size_type
list<T, A>::max_size() const noexcept
{
    return m_node_allocator.max_size();
}


template <typename T, typename A>
void
list<T, A>::clear() noexcept
{
    destroy_list_content();
    init_empty_list();
}

template <typename T, typename A>
typename list<T, A>::iterator_with_operation_error
list<T, A>::insert(const_iterator pos, const value_type& value) noexcept
{
    return emplace(pos, value);
}

template <typename T, typename A>
typename list<T, A>::iterator_with_operation_error
list<T, A>::insert(const_iterator pos, value_type&& value) noexcept
{
    return emplace(pos, std::move(value));
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::insert(const_iterator pos, size_type count, const value_type& value) noexcept
{
    while (count > 0)
    {
        auto err = insert(pos, value);
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
list<T, A>::insert(const_iterator pos, InputIterator first, InputIterator last) noexcept
{
    while (first != last)
    {
        auto err = insert(pos, *first);
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
typename list<T, A>::iterator_with_operation_error
list<T, A>::insert(const_iterator pos, std::initializer_list<T> ilist) noexcept
{
    return insert(pos, ilist.begin(), ilist.end());
}

template <typename T, typename A>
template<typename ... Args>
typename list<T, A>::iterator_with_operation_error
list<T, A>::emplace(const_iterator pos, Args&&... args) noexcept
{
    node_type* newNode;
    auto err = create_node(newNode, std::forward<Args>(args)...);
    if (err)
    {
        return nestl::make_result_with_operation_error(end(), err);
    }

    newNode->inject(pos.get_list_node());

    return nestl::make_result_with_operation_error(iterator(newNode), err);
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::erase(const_iterator pos) noexcept
{
    node_type* node = pos.get_list_node();
    iterator ret = iterator(node->m_next);

    node->remove();

    allocator_type alloc = get_allocator();
    node->destroy_value(alloc);
    m_node_allocator.deallocate(node, 1);

    return ret;
}

template <typename T, typename A>
typename list<T, A>::iterator
list<T, A>::erase(const_iterator first, const_iterator last) noexcept
{
    while (first != last)
    {
        first = erase(first);
    }

    return last.get_list_node();
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_back(const value_type& value) noexcept
{
    return insert(cend(), value).error();
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_back(value_type&& value) noexcept
{
    return insert(cend(), std::move(value)).error();
}

template <typename T, typename A>
template<typename ... Args>
typename list<T, A>::operation_error
list<T, A>::emplace_back(Args&& ... args) noexcept
{
    return emplace(cend(), std::forward<Args>(args) ...).error();
}

template <typename T, typename A>
void
list<T, A>::pop_back() noexcept
{
    erase(iterator(m_node.m_prev));
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_front(const value_type& value) noexcept
{
    return insert(cbegin(), value).error();
}

template <typename T, typename A>
typename list<T, A>::operation_error
list<T, A>::push_front(value_type&& value) noexcept
{
    return insert(cbegin(), std::move(value)).error();
}

template <typename T, typename A>
template<typename ... Args>
typename list<T, A>::operation_error
list<T, A>::emplace_front(Args&& ... args) noexcept
{
    return emplace(cbegin(), std::forward<Args>(args) ...).error();
}

template <typename T, typename A>
void
list<T, A>::pop_front() noexcept
{
    erase(cbegin());
}

/// Private implementation

template <typename T, typename A>
void
list<T, A>::init_empty_list() noexcept
{
    m_node.m_next = &m_node;
    m_node.m_prev = &m_node;
}


template <typename T, typename A>
void
list<T, A>::destroy_list_content() noexcept
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
list<T, A>::swap_data(list& other) noexcept
{
    nestl::detail::list_node_base::swap(other.m_node, m_node);
}

template <typename T, typename A>
void
list<T, A>::move_assign(const std::true_type& /* true_val */, list&& other) noexcept
{
    const list tmp(std::move(*this));
    this->swap_data(other);
}

template <typename T, typename A>
template <typename ... Args>
typename list<T, A>::operation_error
list<T, A>::create_node(node_type*& node, Args&& ... args) noexcept
{
    node_type* tmp = m_node_allocator.allocate(1);
    if (!tmp)
    {
        return operation_error(std::errc::not_enough_memory);
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

} // namespace nestl

#endif /* NESTL_LIST_HPP */
