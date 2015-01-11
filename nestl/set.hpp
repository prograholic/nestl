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

    typedef nestl::result_with_operation_error<iterator, operation_error>           iterator_with_operation_error;
    typedef nestl::result_with_operation_error<iterator_with_flag, operation_error> iterator_with_flag_with_operation_error;


// constructors
    explicit set(const Compare& comp = Compare(), const Alloc& alloc = Alloc()) NESTL_NOEXCEPT_SPEC;
    explicit set(const Alloc& alloc) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    set(set&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

// allocator support
    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC;

// assignment operators and functions
#if NESTL_HAS_RVALUE_REF
    set& operator=(set&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    operation_error assign_copy(const set& other) NESTL_NOEXCEPT_SPEC;


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

    iterator_with_flag_with_operation_error insert(const value_type& val) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    iterator_with_flag_with_operation_error insert(value_type&& val) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    iterator erase(const_iterator pos) NESTL_NOEXCEPT_SPEC;

    size_type erase(const key_type& key) NESTL_NOEXCEPT_SPEC;



// lookup

    iterator find(const Key& key) NESTL_NOEXCEPT_SPEC;

    const_iterator find(const Key& key) const NESTL_NOEXCEPT_SPEC;

private:
    impl_type m_impl;

};


// implementation

template <typename T, typename C, typename A>
set<T, C, A>::set(const C& comp, const A& alloc) NESTL_NOEXCEPT_SPEC
    : m_impl(comp, alloc)
{
}

template <typename T, typename C, typename A>
set<T, C, A>::set(const A& alloc) NESTL_NOEXCEPT_SPEC
    : m_impl(alloc)
{
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename C, typename A>
set<T, C, A>::set(set&& other) NESTL_NOEXCEPT_SPEC
    : m_impl(nestl::move(other.m_impl))
{
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename C, typename A>
typename set<T, C, A>::allocator_type
set<T, C, A>::get_allocator() const NESTL_NOEXCEPT_SPEC
{
    return allocator_type(m_impl.get_allocator());
}



#if NESTL_HAS_RVALUE_REF
template <typename T, typename C, typename A>
set<T, C, A>&
set<T, C, A>::operator=(set&& other) NESTL_NOEXCEPT_SPEC
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
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename C, typename A>
typename set<T, C, A>::operation_error
set<T, C, A>::assign_copy(const set& other) NESTL_NOEXCEPT_SPEC
{
    return m_impl.assign_copy(other.m_impl);
}


// iterators
template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::begin() NESTL_NOEXCEPT_SPEC
{
    return m_impl.begin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::begin() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.begin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::cbegin() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.begin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::end() NESTL_NOEXCEPT_SPEC
{
    return m_impl.end();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::end() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.end();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::cend() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.end();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::reverse_iterator
set<T, C, A>::rbegin() NESTL_NOEXCEPT_SPEC
{
    return m_impl.rbegin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::rbegin() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.rbegin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::crbegin() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.crbegin();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::reverse_iterator
set<T, C, A>::rend() NESTL_NOEXCEPT_SPEC
{
    return m_impl.rend();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::rend() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.rend();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_reverse_iterator
set<T, C, A>::crend() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.crend();
}


// capacity
template <typename T, typename C, typename A>
bool
set<T, C, A>::empty() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.empty();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::size() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.size();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::max_size() const NESTL_NOEXCEPT_SPEC
{
    return m_impl.max_size();
}


// modifiers
template <typename T, typename C, typename A>
void
set<T, C, A>::clear() NESTL_NOEXCEPT_SPEC
{
    m_impl.clear();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator_with_flag_with_operation_error
set<T, C, A>::insert(const value_type& val) NESTL_NOEXCEPT_SPEC
{
    return m_impl.m_insert_unique(val);
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename C, typename A>
typename set<T, C, A>::iterator_with_flag_with_operation_error
set<T, C, A>::insert(value_type&& val) NESTL_NOEXCEPT_SPEC
{
    return m_impl.m_insert_unique(nestl::forward<value_type>(val));
}
#endif /* NESTL_HAS_RVALUE_REF */

template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::erase(const_iterator pos) NESTL_NOEXCEPT_SPEC
{
    return m_impl.erase(pos);
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::erase(const key_type& key) NESTL_NOEXCEPT_SPEC
{
    return m_impl.erase(key);
}



// lookup
template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::find(const T& key) NESTL_NOEXCEPT_SPEC
{
    return m_impl.find(key);
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::find(const T& key) const NESTL_NOEXCEPT_SPEC
{
    return m_impl.find(key);
}




} // namespace nestl


#endif /* NESTL_SET_HPP */
