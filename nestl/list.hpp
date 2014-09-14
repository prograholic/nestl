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

template <typename T>
struct list_iterator
{
    typedef std::ptrdiff_t                  difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                               value_type;
    typedef T*                              pointer;
    typedef T&                              reference;

    reference operator*() const noexcept;

    pointer operator->() const noexcept;

    list_iterator& operator++() noexcept;

    list_iterator operator++(int) noexcept;

    list_iterator& operator--() noexcept;

    list_iterator operator--(int) noexcept;

    bool operator==(const list_iterator& other) const noexcept;

    bool operator!=(const list_iterator& other) const noexcept;
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

    list_const_iterator(const iterator& i) noexcept;

    reference operator*() const noexcept;

    pointer operator->() const noexcept;

    list_const_iterator& operator++() noexcept;

    list_const_iterator operator++(int) noexcept;

    list_const_iterator& operator--() noexcept;

    list_const_iterator operator--(int) noexcept;

    bool operator==(const list_const_iterator& other) const noexcept;

    bool operator!=(const list_const_iterator& other) const noexcept;
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

    iterator_with_operation_error erase(const_iterator pos) noexcept;

    iterator_with_operation_error erase(const_iterator first, const_iterator last) noexcept;

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



} // namespace nestl

#endif /* NESTL_LIST_HPP */
