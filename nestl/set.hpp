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
#include <nestl/functional.hpp>

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

    typedef nestl::detail::identity<Key>  key_of_value;
    typedef nestl::detail::rb_tree<Key, Key, key_of_value, Compare, allocator_type> impl_type;

    typedef typename impl_type::key_type                                            key_type;
    typedef typename impl_type::value_type                                          value_type;

    typedef typename impl_type::size_type                                           size_type;
    typedef typename impl_type::difference_type                                     difference_type;

    typedef typename impl_type::reference_type                                      reference_type;
    typedef typename impl_type::const_reference_type                                const_reference_type;

    typedef typename nestl::allocator_traits<Alloc>::pointer                        pointer;
    typedef typename nestl::allocator_traits<Alloc>::const_pointer                  const_pointer;




    typedef typename impl_type::iterator                                            iterator;
    typedef typename impl_type::const_iterator                                      const_iterator;
    typedef typename impl_type::reverse_iterator                                    reverse_iterator;
    typedef typename impl_type::const_reverse_iterator                              const_reverse_iterator;


    typedef nestl::error_condition                                                  operation_error;

    typedef nestl::pair<iterator, bool>                                             iterator_with_flag;

    typedef nestl::result_with_operation_error<iterator, operation_error>           iterator_with_operation_error;
    typedef nestl::result_with_operation_error<iterator_with_flag, operation_error> iterator_with_flag_with_operation_error;


// constructors
    explicit set(const Compare& comp = Compare(), const Alloc& alloc = Alloc()) NESTL_NOEXCEPT_SPEC;
    explicit set(const Alloc& alloc) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    set(set&& other) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */


// destructor
    ~set() NESTL_NOEXCEPT_SPEC;

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

};





// implementation

template <typename T, typename C, typename A>
set<T, C, A>::set(const C& /* comp */, const A& /* alloc */) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
}

template <typename T, typename C, typename A>
set<T, C, A>::set(const A& /* alloc */) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
}

#if NESTL_HAS_RVALUE_REF
template <typename T, typename C, typename A>
set<T, C, A>::set(set&& /* other */) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
}
#endif /* NESTL_HAS_RVALUE_REF */



template <typename T, typename C, typename A>
set<T, C, A>::~set() NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
}



template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::end() NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
    return iterator();
}


template <typename T, typename C, typename A>
bool
set<T, C, A>::empty() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
    return true;
}

template <typename T, typename C, typename A>
typename set<T, C, A>::size_type
set<T, C, A>::size() const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
    return 0;
}


template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::find(const T& /* key */) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
    return iterator();
}

template <typename T, typename C, typename A>
typename set<T, C, A>::const_iterator
set<T, C, A>::find(const T& /* key */) const NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
    return const_iterator();
}


template <typename T, typename C, typename A>
typename set<T, C, A>::iterator
set<T, C, A>::erase(set<T, C, A>::const_iterator /* pos */) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(0 && "not implemented");
    return iterator();
}




} // namespace nestl


#endif /* NESTL_SET_HPP */
