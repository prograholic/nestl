#ifndef NESTL_SET_HPP
#define NESTL_SET_HPP

#include <nestl/config.hpp>
#include <nestl/functional.hpp>
#include <nestl/allocator.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/cstddef.hpp>
#include <nestl/iterator.hpp>
#include <nestl/system_error.hpp>
#include <nestl/operation_error.hpp>
#include <nestl/detail/red_black_tree.hpp>
#include <nestl/pair.hpp>

namespace nestl
{

namespace detail
{



} // namespace detail


template<typename Key, typename Compare = nestl::less<Key>, typename Alloc = nestl::allocator<Key> >
class set : private nestl::noncopyable
{
public:

    typedef Alloc                                                                   allocator_type;

    typedef nestl::detail::identity<Key>                                            key_of_value;
    typedef nestl::error_condition                                                  operation_error;
    typedef nestl::detail::rb_tree<Key, Key, key_of_value, Compare, operation_error, allocator_type> impl_type;

    typedef typename impl_type::key_type                                            key_type;
    typedef typename impl_type::value_type                                          value_type;

    typedef typename impl_type::size_type                                           size_type;
    typedef typename impl_type::difference_type                                     difference_type;

    typedef typename impl_type::reference                                           reference;
    typedef typename impl_type::const_reference                                     const_reference;

    typedef typename nestl::allocator_traits<Alloc>::pointer                        pointer;
    typedef typename nestl::allocator_traits<Alloc>::const_pointer                  const_pointer;




    typedef typename impl_type::iterator                                            iterator;
    typedef typename impl_type::const_iterator                                      const_iterator;
    typedef typename impl_type::reverse_iterator                                    reverse_iterator;
    typedef typename impl_type::const_reverse_iterator                              const_reverse_iterator;

    typedef nestl::pair<iterator, bool>                                             iterator_with_flag;

// constructors
    explicit set(const Compare& comp = Compare(), const Alloc& alloc = Alloc()) noexcept;
    explicit set(const Alloc& alloc) noexcept;

    set(set&& other) noexcept;

// allocator support
    allocator_type get_allocator() const noexcept;

// assignment operators and functions
    set& operator=(set&& other) noexcept;

    void assign_copy(operation_error& error, const set& other) noexcept;


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

    iterator_with_flag insert_nothrow(operation_error& error, const value_type& val) noexcept;

    iterator_with_flag insert_nothrow(operation_error& error, value_type&& val) noexcept;

    iterator erase(const_iterator pos) noexcept;

    size_type erase(const key_type& key) noexcept;



// lookup

    iterator find(const Key& key) noexcept;

    const_iterator find(const Key& key) const noexcept;

private:
    impl_type m_impl;

};


// implementation

template <typename T, typename C, typename A>
set<T, C, A>::set(const C& comp, const A& alloc) noexcept
    : m_impl(comp, alloc)
{
}

template <typename T, typename C, typename A>
set<T, C, A>::set(const A& alloc) noexcept
    : m_impl(alloc)
{
}

template <typename T, typename C, typename A>
set<T, C, A>::set(set&& other) noexcept
    : m_impl(nestl::move(other.m_impl))
{
}

template <typename T, typename C, typename A>
typename set<T, C, A>::allocator_type
set<T, C, A>::get_allocator() const noexcept
{
    return allocator_type(m_impl.get_allocator());
}

template <typename T, typename C, typename A>
set<T, C, A>&
set<T, C, A>::operator=(set&& other) noexcept
{
    if (!m_impl.m_move_assign(other.m_impl))
    {
        // The rvalue's allocator cannot be moved and is not equal,
        // so we need to individually move each element.
        clear();
        insert(nestl::make_move_iterator(other.m_impl.begin()),
               nestl::make_move_iterator(other.m_impl.end()));
        other.clear();
      }
        return *this;
}

template <typename T, typename C, typename A>
void
set<T, C, A>::assign_copy(operation_error& error, const set& other) noexcept
{
    m_impl.assign_copy(error, other.m_impl);
}


// iterators
template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::begin() noexcept
{
    return m_impl.begin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::begin() const noexcept
{
    return m_impl.begin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::cbegin() const noexcept
{
    return m_impl.begin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::end() noexcept
{
    return m_impl.end();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::end() const noexcept
{
    return m_impl.end();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::cend() const noexcept
{
    return m_impl.end();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::reverse_iterator
set<T, C, A>::rbegin() noexcept
{
    return m_impl.rbegin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::rbegin() const noexcept
{
    return m_impl.rbegin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::crbegin() const noexcept
{
    return m_impl.crbegin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::reverse_iterator
set<T, C, A>::rend() noexcept
{
    return m_impl.rend();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::rend() const noexcept
{
    return m_impl.rend();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::crend() const noexcept
{
    return m_impl.crend();
}


// capacity
template <typename T, typename C, typename A>
bool
set<T, C, A>::empty() const noexcept
{
    return m_impl.empty();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::size() const noexcept
{
    return m_impl.size();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::max_size() const noexcept
{
    return m_impl.max_size();
}


// modifiers
template <typename T, typename C, typename A>
void
set<T, C, A>::clear() noexcept
{
    m_impl.clear();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator_with_flag
set<T, C, A>::insert_nothrow(operation_error& error, const value_type& val) noexcept
{
    return m_impl.m_insert_unique_nothrow(error, val);
}

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator_with_flag
set<T, C, A>::insert_nothrow(operation_error& error, value_type&& val) noexcept
{
    return m_impl.m_insert_unique_nothrow(error, nestl::forward<value_type>(val));
}

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::erase(const_iterator pos) noexcept
{
    return m_impl.erase(pos);
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::erase(const key_type& key) noexcept
{
    return m_impl.erase(key);
}



// lookup
template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::find(const T& key) noexcept
{
    return m_impl.find(key);
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::find(const T& key) const noexcept
{
    return m_impl.find(key);
}




} // namespace nestl


#endif /* NESTL_SET_HPP */
