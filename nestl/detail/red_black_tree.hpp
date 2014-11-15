/**
 * @file Implementation of red-black tree,
 * implementation based on GNU libstdc++ <bits/stl_tree.h> and
 * http://en.literateprograms.org/Red-black_tree_(C)
 */

#ifndef NESTL_DETAIL_RED_BLACK_TREE_HPP
#define NESTL_DETAIL_RED_BLACK_TREE_HPP

#include <nestl/config.hpp>
#include <nestl/alignment.hpp>
#include <nestl/allocator.hpp>
#include <nestl/iterator.hpp>
#include <nestl/allocator_traits.hpp>
#include <nestl/system_error.hpp>
#include <nestl/operation_error.hpp>
#include <nestl/pair.hpp>

#include <bits/stl_tree.h>

namespace nestl
{

namespace detail
{

struct rb_tree_node_base
{
    enum color
    {
        red,
        black
    };

    typedef rb_tree_node_base*       base_ptr;
    typedef const rb_tree_node_base* const_base_ptr;

    static base_ptr get_minimum(base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        while (x->m_left != 0)
        {
            x = x->m_left;
        }

        return x;
    }

    static const_base_ptr get_minimum(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        while (x->m_left != 0)
        {
            x = x->m_left;
        }

        return x;
    }

    static base_ptr get_maximum(base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        while (x->m_right != 0)
        {
            x = x->m_right;
        }

        return x;
    }

    static const_base_ptr get_maximum(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        while (x->m_right != 0)
        {
            x = x->m_right;
        }

        return x;
    }

    static base_ptr increment(base_ptr x) NESTL_NOEXCEPT_SPEC;

    static const_base_ptr increment(const_base_ptr x) NESTL_NOEXCEPT_SPEC;

    static base_ptr decrement(base_ptr x) NESTL_NOEXCEPT_SPEC;

    static const_base_ptr decrement(const_base_ptr x) NESTL_NOEXCEPT_SPEC;

    color m_color;
    base_ptr m_parent;
    base_ptr m_left;
    base_ptr m_right;
};


template <typename T>
struct rb_tree_node : public rb_tree_node_base
{
    typedef rb_tree_node* link_type;

    T* get_data()
    {
        return static_cast<T*>(m_storage.address());
    }


private:
    nestl::aligned_buffer<T> m_storage;
};



template <typename T, typename OperationError>
struct rb_tree_iterator
{
    typedef nestl::ptrdiff_t                  difference_type;
    typedef nestl::bidirectional_iterator_tag iterator_category;
    typedef T                                 value_type;
    typedef T&                                reference;
    typedef T*                                pointer;
    typedef OperationError                    operation_error;



    typedef rb_tree_node_base* base_ptr;
    typedef rb_tree_node<T>*   link_type;

    rb_tree_iterator() NESTL_NOEXCEPT_SPEC
        : m_node()
    {
    }

    explicit rb_tree_iterator(link_type x) NESTL_NOEXCEPT_SPEC
        : m_node(x)
    {
    }

    reference operator*() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(m_node);
        return *static_cast<link_type>(m_node)->get_data();
    }

    pointer operator->() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(m_node);
        return static_cast<link_type>(m_node)->get_data();
    }

    rb_tree_iterator& operator++() NESTL_NOEXCEPT_SPEC
    {
        m_node = rb_tree_node_base::increment(m_node);
        return *this;
    }

    rb_tree_iterator operator++(int) NESTL_NOEXCEPT_SPEC
    {
        rb_tree_iterator tmp = *this;
        m_node = rb_tree_node_base::increment(m_node);
        return tmp;
    }

    rb_tree_iterator& operator--() NESTL_NOEXCEPT_SPEC
    {
        m_node = rb_tree_node_base::decrement(m_node);
        return *this;
    }

    rb_tree_iterator operator--(int) NESTL_NOEXCEPT_SPEC
    {
        rb_tree_iterator tmp = *this;
        m_node = rb_tree_node_base::decrement(m_node);
        return tmp;
    }

    bool operator==(const rb_tree_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        return m_node == other.m_node;
    }

    bool operator!=(const rb_tree_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        return m_node != other.m_node;
    }

    base_ptr m_node;
};


template <typename T, typename OperationError>
struct rb_tree_const_iterator
{
    typedef nestl::ptrdiff_t                    difference_type;
    typedef nestl::bidirectional_iterator_tag   iterator_category;
    typedef T                                   value_type;
    typedef const T&                            reference;
    typedef const T*                            pointer;
    typedef OperationError                      operation_error;

    typedef rb_tree_iterator<T, OperationError> iterator;

    typedef rb_tree_node_base::const_base_ptr   base_ptr;
    typedef const rb_tree_node<T>*              link_type;

    rb_tree_const_iterator() NESTL_NOEXCEPT_SPEC
        : m_node()
    {
    }

    explicit rb_tree_const_iterator(link_type x) NESTL_NOEXCEPT_SPEC
        : m_node(x)
    {
    }

    rb_tree_const_iterator(const iterator& it) NESTL_NOEXCEPT_SPEC
        : m_node(it.get_node())
    {
    }

    iterator cast_to_iterator() const NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(m_node);
        return iterator(
                    static_cast<typename iterator::link_type>(
                        const_cast<typename iterator::base_ptr>(m_node)));
    }

    reference operator*() const NESTL_NOEXCEPT_SPEC
    {
        return *static_cast<link_type>(m_node)->get_data();
    }

    pointer operator->() const NESTL_NOEXCEPT_SPEC
    {
        return static_cast<link_type>(m_node)->get_data();
    }

    rb_tree_const_iterator& operator++() NESTL_NOEXCEPT_SPEC
    {
        m_node = rb_tree_node_base::increment(m_node);
        return *this;
    }

    rb_tree_const_iterator operator++(int) NESTL_NOEXCEPT_SPEC
    {
        rb_tree_const_iterator tmp = *this;
        m_node = rb_tree_node_base::increment(m_node);
        return tmp;
    }

    rb_tree_const_iterator& operator--() NESTL_NOEXCEPT_SPEC
    {
        m_node = rb_tree_node_base::decrement(m_node);
        return *this;
    }

    rb_tree_const_iterator operator--(int) NESTL_NOEXCEPT_SPEC
    {
        rb_tree_const_iterator tmp = *this;
        m_node = rb_tree_node_base::decrement(m_node);
        return tmp;
    }

    bool operator==(const rb_tree_const_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        return m_node == other.m_node;
    }

    bool operator!=(const rb_tree_const_iterator& other) const NESTL_NOEXCEPT_SPEC
    {
        return m_node != other.m_node;
    }

    base_ptr m_node;
};


template <typename T, typename OperationError>
bool operator ==(const rb_tree_iterator<T, OperationError>& left,
                 const rb_tree_const_iterator<T, OperationError>& right) NESTL_NOEXCEPT_SPEC
{
    return left.get_node() == right.get_node();
}

template <typename T, typename OperationError>
bool operator !=(const rb_tree_iterator<T, OperationError>& left,
                 const rb_tree_const_iterator<T, OperationError>& right) NESTL_NOEXCEPT_SPEC
{
    return left.get_node() != right.get_node();
}




template <typename Key,
          typename Value,
          typename KeyOfValue,
          typename Compare,
          typename OperationError,
          typename Alloc = nestl::allocator<Value> >
class rb_tree
{
public:

    typedef Key                                                                     key_type;
    typedef Value                                                                   value_type;

    typedef nestl::size_t                                                           size_type;
    typedef nestl::ptrdiff_t                                                        difference_type;

    typedef value_type* 			                                                pointer;
    typedef const value_type* 		                                                const_pointer;
    typedef value_type&                                                             reference;
    typedef const value_type&                                                       const_reference;

    typedef rb_tree_iterator<value_type, operation_error>                           iterator;
    typedef rb_tree_const_iterator<value_type, operation_error>                     const_iterator;
    typedef nestl::reverse_iterator<iterator>                                       reverse_iterator;
    typedef nestl::reverse_iterator<const_iterator>                                 const_reverse_iterator;

    typedef nestl::allocator_rebind<Alloc, rb_tree_node<Value> >                    node_allocator;
    typedef nestl::allocator_traits<node_allocator>                                 node_allocator_traits;
    typedef Alloc                                                                   allocator_type;

    typedef rb_tree_node<Value>*                                                    link_type;
    typedef const rb_tree_node<Value>*                                              const_link_type;

    typedef OperationError                                                          operation_error;
    typedef nestl::pair<iterator, bool>                                             iterator_with_flag;

    typedef nestl::result_with_operation_error<iterator, operation_error>           iterator_with_operation_error;
    typedef nestl::result_with_operation_error<iterator_with_flag, operation_error> iterator_with_flag_with_operation_error;



    node_allocator& get_node_allocator() NESTL_NOEXCEPT_SPEC
    {
        return *static_cast<node_allocator*>(&this->m_impl);
    }

    const node_allocator& get_node_allocator() const NESTL_NOEXCEPT_SPEC
    {
        return *static_cast<const node_allocator*>(&this->m_impl);
    }

    allocator_type get_allocator() const NESTL_NOEXCEPT_SPEC
    {
        return allocator_type(get_node_allocator());
    }

private:

    typedef rb_tree_node_base* 		                             base_ptr;
    typedef const rb_tree_node_base* 	                         const_base_ptr;


#if 0
    operation_error allocate_node(link_type& l) NESTL_NOEXCEPT_SPEC
    {
        l = node_allocator_traits::allocate(get_node_allocator(), 1);
        if (!l)
        {
            return operation_error(nestl::errc::not_enough_memory);
        }

        return operation_error();
    }

    void deallocate_node(link_type p) NESTL_NOEXCEPT_SPEC
    {
        node_allocator_traits::deallocate(get_node_allocator(), p, 1);
    }

#endif //0

#if NESTL_HAS_RVALUE_REF
    template <typename ... Args>
    operation_error create_node(link_type& res, Args&& ... args)
    {
        node_allocator& node_alloc = get_node_allocator();
        res = node_allocator_traits::allocate(node_alloc, 1);
        if (!res)
        {
            return operation_error(nestl::errc::not_enough_memory);
        }

        nestl::detail::allocation_scoped_guard<link_type, node_allocator> guard(node_alloc, res, 1);

        error = nestl::detail::construct<operation_error>(res, nestl::forward<Args>(args)...);
        if (error)
        {
            return error;
        }

        guard.release();
        return error;
    }

#else /* NESTL_HAS_RVALUE_REF */

    operation_error create_node(link_type& res, const value_type& x)
    {
        node_allocator& node_alloc = get_node_allocator();
        res = node_allocator_traits::allocate(node_alloc, 1);
        if (!res)
        {
            return operation_error(nestl::errc::not_enough_memory);
        }

        nestl::detail::allocation_scoped_guard<link_type, node_allocator> guard(node_alloc, res, 1);

        error = nestl::detail::construct<operation_error>(res, x);
        if (error)
        {
            return error;
        }

        guard.release();
        return error;
    }

#endif /* NESTL_HAS_RVALUE_REF */

    void destroy_node(link_type p) NESTL_NOEXCEPT_SPEC
    {
        node_allocator& node_alloc = get_node_allocator();
        node_allocator_traits::destroy(node_alloc, p->get_data());
        p->~rb_tree_node<Value>();
        node_allocator_traits::deallocate(node_alloc, p, 1);
    }

    operation_error clone_node(link_type& res, const_link_type other) NESTL_NOEXCEPT_SPEC
    {
        operation_error err = create_node(res, *other->get_data());
        if (err)
        {
            return err;
        }

        res->m_color = other->m_color;
        res->m_left = 0;
        res->m_right = 0;

        return err;
    }

    template<typename KeyCompare/*,
         bool _Is_pod_comparator = __is_pod(_Key_compare)*/>
    struct rb_tree_impl : public node_allocator
    {
        KeyCompare m_key_compare;
        rb_tree_node_base m_header;
        size_type m_node_count; // Keeps track of size of tree.

        rb_tree_impl() NESTL_NOEXCEPT_SPEC
            : node_allocator()
            , m_key_compare()
            , m_header()
            , m_node_count(0)
        {
            initialize_header();
        }

        rb_tree_impl(const KeyCompare& comp, const node_allocator& a) NESTL_NOEXCEPT_SPEC
            : node_allocator(a)
            , m_key_compare(comp)
            , m_header()
            , m_node_count(0)
        {
            initialize_header();
        }

#if NESTL_HAS_RVALUE_REF
        rb_tree_impl(const KeyCompare& comp, node_allocator&& a) NESTL_NOEXCEPT_SPEC
            : node_allocator(nestl::move(a))
            , m_key_compare(comp)
            , m_header()
            , m_node_count(0)
        {
            initialize_header();
        }
#endif /* NESTL_HAS_RVALUE_REF */

    private:
        void initialize_header() NESTL_NOEXCEPT_SPEC
        {
            m_header.m_color = rb_tree_node_base::red;
            m_header.m_parent = 0;
            m_header.m_left = &m_header;
            m_header.m_right = &m_header;
        }
    };

    rb_tree_impl<Compare> m_impl;

    base_ptr& get_root() NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_header.m_parent;
    }

    const_base_ptr get_root() const NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_header.m_parent;
    }

    base_ptr& get_leftmost() NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_header.m_left;
    }

    const_base_ptr get_leftmost() const NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_header.m_left;
    }

    base_ptr& get_rightmost() NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_header.m_right;
    }

    const_base_ptr get_rightmost() const NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_header.m_right;
    }

    link_type link_begin() NESTL_NOEXCEPT_SPEC
    {
        return static_cast<link_type>(m_impl.m_header.m_parent);
    }

    const_link_type link_begin() const NESTL_NOEXCEPT_SPEC
    {
        return static_cast<const_link_type>(m_impl.m_header.m_parent);
    }

    link_type link_end() NESTL_NOEXCEPT_SPEC
    {
        return static_cast<link_type>(&m_impl.m_header);
    }

    const_link_type link_end() const NESTL_NOEXCEPT_SPEC
    {
        return static_cast<const_link_type>(&m_impl.m_header);
    }

    static const_reference get_value(const_link_type x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return *x->get_data();
    }

    static const Key& get_key(const_link_type x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return KeyOfValue()(get_value(x));
    }

    static link_type get_left(base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return static_cast<link_type>(x->m_left);
    }

    static const_link_type get_left(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return static_cast<const_link_type>(x->m_left);
    }

    static link_type get_right(base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return static_cast<link_type>(x->m_right);
    }

    static const_link_type get_right(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return static_cast<const_link_type>(x->m_right);
    }

    static const_reference get_value(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return *static_cast<const_link_type>(x)->get_data();
    }

    static const Key& get_key(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return KeyOfValue()(get_value(x));
    }

    static base_ptr get_minimum(base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return rb_tree_node_base::get_minimum(x);
    }

    static const_base_ptr get_minimum(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return rb_tree_node_base::get_minimum(x);
    }

    static base_ptr get_maximum(base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return rb_tree_node_base::get_maximum(x);
    }

    static const_base_ptr get_maximum(const_base_ptr x) NESTL_NOEXCEPT_SPEC
    {
        NESTL_ASSERT(x);
        return rb_tree_node_base::get_maximum(x);
    }

private:
    nestl::pair<base_ptr, base_ptr> get_insert_unique_pos(const key_type& k) NESTL_NOEXCEPT_SPEC;

    nestl::pair<base_ptr, base_ptr> get_insert_equal_pos(const key_type& k) NESTL_NOEXCEPT_SPEC;

    nestl::pair<base_ptr, base_ptr> get_insert_hint_unique_pos(const_iterator pos, const key_type& k) NESTL_NOEXCEPT_SPEC;

    nestl::pair<base_ptr, base_ptr> get_insert_hint_equal_pos(const_iterator pos, const key_type& k) NESTL_NOEXCEPT_SPEC;

#if NESTL_HAS_RVALUE_REF
    template<typename Arg>
    iterator_with_operation_error insert(base_ptr x, base_ptr y, Arg&& v) NESTL_NOEXCEPT_SPEC;

    template<typename Arg>
    iterator_with_operation_error insert_lower(base_ptr y, Arg&& v) NESTL_NOEXCEPT_SPEC;

    template<typename Arg>
    iterator_with_operation_error insert_equal_lower(Arg&& x) NESTL_NOEXCEPT_SPEC;

#else /* NESTL_HAS_RVALUE_REF */
    iterator_with_operation_error insert(base_ptr x, base_ptr y, const value_type& v) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert_lower(base_ptr y, const value_type& v) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert_equal_lower(const value_type& x) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    iterator insert_node(base_ptr x, base_ptr y, link_type z) NESTL_NOEXCEPT_SPEC;

    iterator insert_lower_node(base_ptr p, link_type z) NESTL_NOEXCEPT_SPEC;

    iterator insert_equal_lower_node(link_type z) NESTL_NOEXCEPT_SPEC;

    link_type copy(const_link_type x, link_type p) NESTL_NOEXCEPT_SPEC;

    void erase(link_type x) NESTL_NOEXCEPT_SPEC;

    iterator lower_bound(link_type x, link_type y, const Key& k) NESTL_NOEXCEPT_SPEC;

    const_iterator lower_bound(const_link_type x, const_link_type y, const Key& k) const NESTL_NOEXCEPT_SPEC;

    iterator upper_bound(link_type x, link_type y, const Key& k) NESTL_NOEXCEPT_SPEC;

    const_iterator upper_bound(const_link_type x, const_link_type y, const Key& k) const NESTL_NOEXCEPT_SPEC;

public:
    // allocation/deallocation
    rb_tree() NESTL_NOEXCEPT_SPEC
        : m_impl()
    {
    }

    rb_tree(const Compare& comp, const allocator_type& a = allocator_type()) NESTL_NOEXCEPT_SPEC
        : m_impl(comp, node_allocator(a))
    {
    }

    rb_tree(const allocator_type& a) NESTL_NOEXCEPT_SPEC
        : m_impl(Compare(), node_allocator(a))
    {
    }

#if NESTL_HAS_RVALUE_REF

    rb_tree(rb_tree&& other) NESTL_NOEXCEPT_SPEC
        : m_impl(other.m_impl.m_key_compare, other.get_node_allocator())
    {
        if (other.get_root() != 0)
        {
            move_data(other, nestl::true_type());
        }
    }

    rb_tree(rb_tree&& other, const allocator_type& a) NESTL_NOEXCEPT_SPEC
        // @todo Do we really support delegating ctors?
        : rb_tree(nestl::move(other), node_allocator(a))
    {
    }

    rb_tree(rb_tree&& other, node_allocator&& a) NESTL_NOEXCEPT_SPEC;

#endif /* NESTL_HAS_RVALUE_REF */

    ~rb_tree() NESTL_NOEXCEPT_SPEC
    {
        erase(link_begin());
    }

    // Accessors.
    Compare key_comp() const NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_key_compare;
    }

    iterator begin() NESTL_NOEXCEPT_SPEC
    {
        return iterator(static_cast<link_type>(m_impl.m_header.m_left));
    }

    const_iterator begin() const NESTL_NOEXCEPT_SPEC
    {
        return const_iterator(static_cast<const_link_type>(m_impl.m_header.m_left));
    }

    iterator end() NESTL_NOEXCEPT_SPEC
    {
        return iterator(static_cast<link_type>(&m_impl.m_header));
    }

    const_iterator end() const NESTL_NOEXCEPT_SPEC
    {
        return const_iterator(static_cast<const_link_type>(&m_impl.m_header));
    }

    reverse_iterator rbegin() NESTL_NOEXCEPT_SPEC
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const NESTL_NOEXCEPT_SPEC
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() NESTL_NOEXCEPT_SPEC
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const NESTL_NOEXCEPT_SPEC
    {
        return const_reverse_iterator(begin());
    }

    bool empty() const NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_node_count == 0;
    }

    size_type size() const NESTL_NOEXCEPT_SPEC
    {
        return m_impl.m_node_count;
    }

    size_type max_size() const NESTL_NOEXCEPT_SPEC
    {
        return node_allocator_traits::max_size(get_node_allocator());
    }

    void swap(rb_tree& other) NESTL_NOEXCEPT_SPEC;

    // Insert/erase.
#if NESTL_HAS_RVALUE_REF
    template<typename Arg>
    iterator_with_flag_with_operation_error insert_unique(Arg&& x) NESTL_NOEXCEPT_SPEC;

    template<typename Arg>
    iterator_with_operation_error insert_equal(Arg&& x) NESTL_NOEXCEPT_SPEC;

    template<typename Arg>
    iterator_with_operation_error insert_unique_at(const_iterator position, Arg&& x) NESTL_NOEXCEPT_SPEC;

    template<typename Arg>
    iterator_with_operation_error insert_equal_at(const_iterator position, Arg&& x) NESTL_NOEXCEPT_SPEC;

    template<typename... Args>
    iterator_with_flag_with_operation_error emplace_unique(Args&&... args) NESTL_NOEXCEPT_SPEC;

    template<typename... Args>
    iterator_with_operation_error emplace_equal(Args&&... args) NESTL_NOEXCEPT_SPEC;

    template<typename... Args>
    iterator_with_operation_error emplace_hint_unique(const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC;

    template<typename... Args>
    iterator_with_operation_error emplace_hint_equal(const_iterator pos, Args&&... args) NESTL_NOEXCEPT_SPEC;

#else /* NESTL_HAS_RVALUE_REF */
    iterator_with_flag_with_operation_error insert_unique(const value_type& x) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert_equal(const value_type& x) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert_unique_at(const_iterator position, const value_type& x) NESTL_NOEXCEPT_SPEC;

    iterator_with_operation_error insert_equal_at(const_iterator position, const value_type& x) NESTL_NOEXCEPT_SPEC;
#endif /* NESTL_HAS_RVALUE_REF */

    template<typename InputIterator>
    operation_error insert_unique(InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

    template<typename InputIterator>
    operation_error insert_equal(InputIterator first, InputIterator last) NESTL_NOEXCEPT_SPEC;

private:
    void erase_aux(const_iterator position) NESTL_NOEXCEPT_SPEC;

    void erase_aux(const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC;

public:

    iterator erase(const_iterator position) NESTL_NOEXCEPT_SPEC
    {
        const_iterator result = position;
        ++result;
        erase_aux(position);
        return result.cast_to_iterator();
    }

    iterator erase(iterator position) NESTL_NOEXCEPT_SPEC
    {
        iterator result = position;
        ++result;
        erase_aux(position);
        return result;
    }

    size_type erase(const key_type& x) NESTL_NOEXCEPT_SPEC;

    iterator erase(const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC
    {
        erase_aux(first, last);
        return last.cast_to_iterator();
    }

    void erase(const key_type* first, const key_type* last) NESTL_NOEXCEPT_SPEC;

    void clear() NESTL_NOEXCEPT_SPEC
    {
        erase(link_begin());
        get_leftmost() = link_end();
        get_root() = 0;
        get_rightmost() = link_end();
        m_impl.m_node_count = 0;
    }

    // Set operations.
    iterator find(const key_type& k) NESTL_NOEXCEPT_SPEC;

    const_iterator find(const key_type& k) const NESTL_NOEXCEPT_SPEC;

    size_type count(const key_type& k) const NESTL_NOEXCEPT_SPEC;

    iterator lower_bound(const key_type& k) NESTL_NOEXCEPT_SPEC
    {
        return lower_bound(link_begin(), link_end(), k);
    }

    const_iterator lower_bound(const key_type& __k) const NESTL_NOEXCEPT_SPEC
    {
        return lower_bound(link_begin(), link_end(), k);
    }

    iterator upper_bound(const key_type& k) NESTL_NOEXCEPT_SPEC
    {
        return upper_bound(link_begin(), link_end(), k);
    }

    const_iterator upper_bound(const key_type& k) const NESTL_NOEXCEPT_SPEC
    {
        return upper_bound(link_begin(), link_end(), k);
    }

    nestl::pair<iterator, iterator> equal_range(const key_type& k) NESTL_NOEXCEPT_SPEC;

    nestl::pair<const_iterator, const_iterator> equal_range(const key_type& k) const NESTL_NOEXCEPT_SPEC;

    // Debugging.
    bool rb_verify() const NESTL_NOEXCEPT_SPEC;

    bool move_assign(rb_tree& other) NESTL_NOEXCEPT_SPEC;

private:
    // Move elements from container with equal allocator.
    void move_data(rb_tree& other, nestl::true_type) NESTL_NOEXCEPT_SPEC;
};

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline bool operator==(const rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                       const rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    return (x.size() == y.size()) && nestl::equal(x.begin(), x.end(), y.begin());
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline bool operator<(const rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                      const rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    return nestl::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline bool operator!=(const rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                       const rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    return !(x == y);
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline bool operator>(const rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                      const rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    return y < x;
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline bool operator<=(const rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                       const rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    return !(y < x);
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline bool operator>=(const rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                       const rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    return !(x < y);
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
inline void swap(rb_tree<K, V, KV, Cmp, OpErr, A>& x,
                 rb_tree<K, V, KV, Cmp, OpErr, A>& y) NESTL_NOEXCEPT_SPEC
{
    x.swap(y);
}

#if NESTL_HAS_RVALUE_REF
template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
rb_tree<K, V, KV, Cmp, OpErr, A>::rb_tree(b_tree&& x, node_allocator&& a)
    : m_impl(x.m_impl.m_key_compare, nestl::move(a))
{
    typedef = nestl::integral_constant<bool, node_allocator_traits::always_equal()> eq_type;
    if (x.get_root() != 0)
    {
        move_data(x, eq_type());
    }
}

#endif /* NESTL_HAS_RVALUE_REF */

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
void rb_tree<K, V, KV, Cmp, OpErr, A>::move_data(rb_tree& other, nestl::true_type)
{
    get_root() = other.get_root();
    get_leftmost() = other.get_leftmost();
    get_rightmost() = other.get_rightmost();
    get_root()->m_parent = link_end();

    other.get_root() = 0;
    other.get_leftmost() = other.link_end();
    other.get_rightmost() = other.link_end();

    m_impl.m_node_count = other.m_impl.m_node_count;
    other.m_impl.m_node_count = 0;
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
bool rb_tree<K, V, KV, Cmp, OpErr, A>::move_assign(rb_tree& other)
{
    m_impl.m_key_compare = other.m_impl.m_key_compare;

    if (node_allocator_traits::propagate_on_move_assign()
        || node_allocator_traits::always_equal()
        || (get_node_allocator() == other.get_node_allocator()))
    {
        clear();
        if (other.get_root() != 0)
        {
            move_data(other, nestl::true_type());
        }
        std::__alloc_on_move(_M_get_Node_allocator(),
               other._M_get_Node_allocator());
        return true;
    }
    return false;
}

template<typename K, typename V, typename KV, typename Cmp, typename OpErr, typename A>
#if NESTL_HAS_RVALUE_REF

template<typename _Arg>
typename rb_tree<K, V, KV, Cmp, OpErr, A>::iterator_with_operation_error
rb_tree<K, V, KV, Cmp, OpErr, A>::insert(base_ptr x, base_ptr p, Arg&& v)
{
    bool insert_left = ((x != 0)
                        || (p == link_end())
                        || (m_impl.key_compare(KV()(v), get_key(p)));

    link_type z;
    operation_error error = create_node(z, nestl::forward<Arg>(v));
    if (error)
    {
        return nestl::make_result_with_operation_error(iterator(), error);
    }

    rb_tree_insert_and_rebalance(insert_left, z, p, m_impl.m_header);
    ++m_impl.m_node_count;

    return nestl::make_result_with_operation_error(iterator(z), error);
}

#else /* NESTL_HAS_RVALUE_REF */

typename rb_tree<K, V, KV, Cmp, OpErr, A>::iterator_with_operation_error
rb_tree<K, V, KV, Cmp, OpErr, A>::insert(base_ptr x, base_ptr p, const V& v)
{
    bool insert_left = ((x != 0)
                        || (p == link_end())
                        || (m_impl.key_compare(KV()(v), get_key(p)));

    link_type z;
    operation_error error = create_node(z, v);
    if (error)
    {
        return nestl::make_result_with_operation_error(iterator(), error);
    }

    rb_tree_insert_and_rebalance(insert_left, z, p, m_impl.m_header);
    ++m_impl.m_node_count;

    return nestl::make_result_with_operation_error(iterator(z), error);
}

#endif /* NESTL_HAS_RVALUE_REF */


template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
template<typename _Arg>
#endif
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
_M_insert_lower(_Base_ptr __p, _Arg&& __v)
#else
_M_insert_lower(_Base_ptr __p, const _Val& __v)
#endif
{
  bool __insert_left = (__p == _M_end()
            || !_M_impl._M_key_compare(_S_key(__p),
                           _KeyOfValue()(__v)));

  _Link_type __z = _M_create_node(_GLIBCXX_FORWARD(_Arg, __v));

  _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,
                this->_M_impl._M_header);
  ++_M_impl._M_node_count;
  return iterator(__z);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
template<typename _Arg>
#endif
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
_M_insert_equal_lower(_Arg&& __v)
#else
_M_insert_equal_lower(const _Val& __v)
#endif
{
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  while (__x != 0)
{
  __y = __x;
  __x = !_M_impl._M_key_compare(_S_key(__x), _KeyOfValue()(__v)) ?
        _S_left(__x) : _S_right(__x);
}
  return _M_insert_lower(__y, _GLIBCXX_FORWARD(_Arg, __v));
}

template<typename _Key, typename _Val, typename _KoV,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::_Link_type
_Rb_tree<_Key, _Val, _KoV, _Compare, _Alloc>::
_M_copy(_Const_Link_type __x, _Link_type __p)
{
  // Structural copy.  __x and __p must be non-null.
  _Link_type __top = _M_clone_node(__x);
  __top->_M_parent = __p;

  __try
{
  if (__x->_M_right)
    __top->_M_right = _M_copy(_S_right(__x), __top);
  __p = __top;
  __x = _S_left(__x);

  while (__x != 0)
    {
      _Link_type __y = _M_clone_node(__x);
      __p->_M_left = __y;
      __y->_M_parent = __p;
      if (__x->_M_right)
    __y->_M_right = _M_copy(_S_right(__x), __y);
      __p = __y;
      __x = _S_left(__x);
    }
}
  __catch(...)
{
  _M_erase(__top);
  __throw_exception_again;
}
  return __top;
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
void
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_erase(_Link_type __x)
{
  // Erase without rebalancing.
  while (__x != 0)
{
  _M_erase(_S_right(__x));
  _Link_type __y = _S_left(__x);
  _M_destroy_node(__x);
  __x = __y;
}
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue,
          _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_lower_bound(_Link_type __x, _Link_type __y,
       const _Key& __k)
{
  while (__x != 0)
if (!_M_impl._M_key_compare(_S_key(__x), __k))
  __y = __x, __x = _S_left(__x);
else
  __x = _S_right(__x);
  return iterator(__y);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue,
          _Compare, _Alloc>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_lower_bound(_Const_Link_type __x, _Const_Link_type __y,
       const _Key& __k) const
{
  while (__x != 0)
if (!_M_impl._M_key_compare(_S_key(__x), __k))
  __y = __x, __x = _S_left(__x);
else
  __x = _S_right(__x);
  return const_iterator(__y);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue,
          _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_upper_bound(_Link_type __x, _Link_type __y,
       const _Key& __k)
{
  while (__x != 0)
if (_M_impl._M_key_compare(__k, _S_key(__x)))
  __y = __x, __x = _S_left(__x);
else
  __x = _S_right(__x);
  return iterator(__y);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue,
          _Compare, _Alloc>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_upper_bound(_Const_Link_type __x, _Const_Link_type __y,
       const _Key& __k) const
{
  while (__x != 0)
if (_M_impl._M_key_compare(__k, _S_key(__x)))
  __y = __x, __x = _S_left(__x);
else
  __x = _S_right(__x);
  return const_iterator(__y);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::iterator,
 typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::iterator>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
equal_range(const _Key& __k)
{
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  while (__x != 0)
{
  if (_M_impl._M_key_compare(_S_key(__x), __k))
    __x = _S_right(__x);
  else if (_M_impl._M_key_compare(__k, _S_key(__x)))
    __y = __x, __x = _S_left(__x);
  else
    {
      _Link_type __xu(__x), __yu(__y);
      __y = __x, __x = _S_left(__x);
      __xu = _S_right(__xu);
      return pair<iterator,
              iterator>(_M_lower_bound(__x, __y, __k),
                _M_upper_bound(__xu, __yu, __k));
    }
}
  return pair<iterator, iterator>(iterator(__y),
                  iterator(__y));
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::const_iterator,
 typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::const_iterator>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
equal_range(const _Key& __k) const
{
  _Const_Link_type __x = _M_begin();
  _Const_Link_type __y = _M_end();
  while (__x != 0)
{
  if (_M_impl._M_key_compare(_S_key(__x), __k))
    __x = _S_right(__x);
  else if (_M_impl._M_key_compare(__k, _S_key(__x)))
    __y = __x, __x = _S_left(__x);
  else
    {
      _Const_Link_type __xu(__x), __yu(__y);
      __y = __x, __x = _S_left(__x);
      __xu = _S_right(__xu);
      return pair<const_iterator,
              const_iterator>(_M_lower_bound(__x, __y, __k),
                  _M_upper_bound(__xu, __yu, __k));
    }
}
  return pair<const_iterator, const_iterator>(const_iterator(__y),
                      const_iterator(__y));
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
void
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
swap(_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>& __t)
#if __cplusplus >= 201103L
noexcept(_Alloc_traits::_S_nothrow_swap())
#endif
{
  if (_M_root() == 0)
{
  if (__t._M_root() != 0)
    {
      _M_root() = __t._M_root();
      _M_leftmost() = __t._M_leftmost();
      _M_rightmost() = __t._M_rightmost();
      _M_root()->_M_parent = _M_end();

      __t._M_root() = 0;
      __t._M_leftmost() = __t._M_end();
      __t._M_rightmost() = __t._M_end();
    }
}
  else if (__t._M_root() == 0)
{
  __t._M_root() = _M_root();
  __t._M_leftmost() = _M_leftmost();
  __t._M_rightmost() = _M_rightmost();
  __t._M_root()->_M_parent = __t._M_end();

  _M_root() = 0;
  _M_leftmost() = _M_end();
  _M_rightmost() = _M_end();
}
  else
{
  std::swap(_M_root(),__t._M_root());
  std::swap(_M_leftmost(),__t._M_leftmost());
  std::swap(_M_rightmost(),__t._M_rightmost());

  _M_root()->_M_parent = _M_end();
  __t._M_root()->_M_parent = __t._M_end();
}
  // No need to swap header's color as it does not change.
  std::swap(this->_M_impl._M_node_count, __t._M_impl._M_node_count);
  std::swap(this->_M_impl._M_key_compare, __t._M_impl._M_key_compare);

  _Alloc_traits::_S_on_swap(_M_get_Node_allocator(),
            __t._M_get_Node_allocator());
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr,
 typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_get_insert_unique_pos(const key_type& __k)
{
  typedef pair<_Base_ptr, _Base_ptr> _Res;
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  bool __comp = true;
  while (__x != 0)
{
  __y = __x;
  __comp = _M_impl._M_key_compare(__k, _S_key(__x));
  __x = __comp ? _S_left(__x) : _S_right(__x);
}
  iterator __j = iterator(__y);
  if (__comp)
{
  if (__j == begin())
    return _Res(__x, __y);
  else
    --__j;
}
  if (_M_impl._M_key_compare(_S_key(__j._M_node), __k))
return _Res(__x, __y);
  return _Res(__j._M_node, 0);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr,
 typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_get_insert_equal_pos(const key_type& __k)
{
  typedef pair<_Base_ptr, _Base_ptr> _Res;
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  while (__x != 0)
{
  __y = __x;
  __x = _M_impl._M_key_compare(__k, _S_key(__x)) ?
        _S_left(__x) : _S_right(__x);
}
  return _Res(__x, __y);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
template<typename _Arg>
#endif
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::iterator, bool>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
_M_insert_unique(_Arg&& __v)
#else
_M_insert_unique(const _Val& __v)
#endif
{
  typedef pair<iterator, bool> _Res;
  pair<_Base_ptr, _Base_ptr> __res
= _M_get_insert_unique_pos(_KeyOfValue()(__v));

  if (__res.second)
return _Res(_M_insert_(__res.first, __res.second,
               _GLIBCXX_FORWARD(_Arg, __v)),
        true);

  return _Res(iterator(static_cast<_Link_type>(__res.first)), false);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
template<typename _Arg>
#endif
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
_M_insert_equal(_Arg&& __v)
#else
_M_insert_equal(const _Val& __v)
#endif
{
  pair<_Base_ptr, _Base_ptr> __res
= _M_get_insert_equal_pos(_KeyOfValue()(__v));
  return _M_insert_(__res.first, __res.second, _GLIBCXX_FORWARD(_Arg, __v));
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr,
     typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_get_insert_hint_unique_pos(const_iterator __position,
              const key_type& __k)
{
  iterator __pos = __position._M_const_cast();
  typedef pair<_Base_ptr, _Base_ptr> _Res;

  // end()
  if (__pos._M_node == _M_end())
{
  if (size() > 0
      && _M_impl._M_key_compare(_S_key(_M_rightmost()), __k))
    return _Res(0, _M_rightmost());
  else
    return _M_get_insert_unique_pos(__k);
}
  else if (_M_impl._M_key_compare(__k, _S_key(__pos._M_node)))
{
  // First, try before...
  iterator __before = __pos;
  if (__pos._M_node == _M_leftmost()) // begin()
    return _Res(_M_leftmost(), _M_leftmost());
  else if (_M_impl._M_key_compare(_S_key((--__before)._M_node), __k))
    {
      if (_S_right(__before._M_node) == 0)
    return _Res(0, __before._M_node);
      else
    return _Res(__pos._M_node, __pos._M_node);
    }
  else
    return _M_get_insert_unique_pos(__k);
}
  else if (_M_impl._M_key_compare(_S_key(__pos._M_node), __k))
{
  // ... then try after.
  iterator __after = __pos;
  if (__pos._M_node == _M_rightmost())
    return _Res(0, _M_rightmost());
  else if (_M_impl._M_key_compare(__k, _S_key((++__after)._M_node)))
    {
      if (_S_right(__pos._M_node) == 0)
    return _Res(0, __pos._M_node);
      else
    return _Res(__after._M_node, __after._M_node);
    }
  else
    return _M_get_insert_unique_pos(__k);
}
  else
// Equivalent keys.
return _Res(__pos._M_node, 0);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
template<typename _Arg>
#endif
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
_M_insert_unique_(const_iterator __position, _Arg&& __v)
#else
_M_insert_unique_(const_iterator __position, const _Val& __v)
#endif
{
  pair<_Base_ptr, _Base_ptr> __res
= _M_get_insert_hint_unique_pos(__position, _KeyOfValue()(__v));

  if (__res.second)
return _M_insert_(__res.first, __res.second,
          _GLIBCXX_FORWARD(_Arg, __v));
  return iterator(static_cast<_Link_type>(__res.first));
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr,
     typename _Rb_tree<_Key, _Val, _KeyOfValue,
           _Compare, _Alloc>::_Base_ptr>
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_get_insert_hint_equal_pos(const_iterator __position, const key_type& __k)
{
  iterator __pos = __position._M_const_cast();
  typedef pair<_Base_ptr, _Base_ptr> _Res;

  // end()
  if (__pos._M_node == _M_end())
{
  if (size() > 0
      && !_M_impl._M_key_compare(__k, _S_key(_M_rightmost())))
    return _Res(0, _M_rightmost());
  else
    return _M_get_insert_equal_pos(__k);
}
  else if (!_M_impl._M_key_compare(_S_key(__pos._M_node), __k))
{
  // First, try before...
  iterator __before = __pos;
  if (__pos._M_node == _M_leftmost()) // begin()
    return _Res(_M_leftmost(), _M_leftmost());
  else if (!_M_impl._M_key_compare(__k, _S_key((--__before)._M_node)))
    {
      if (_S_right(__before._M_node) == 0)
    return _Res(0, __before._M_node);
      else
    return _Res(__pos._M_node, __pos._M_node);
    }
  else
    return _M_get_insert_equal_pos(__k);
}
  else
{
  // ... then try after.
  iterator __after = __pos;
  if (__pos._M_node == _M_rightmost())
    return _Res(0, _M_rightmost());
  else if (!_M_impl._M_key_compare(_S_key((++__after)._M_node), __k))
    {
      if (_S_right(__pos._M_node) == 0)
    return _Res(0, __pos._M_node);
      else
    return _Res(__after._M_node, __after._M_node);
    }
  else
    return _Res(0, 0);
}
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
#if __cplusplus >= 201103L
template<typename _Arg>
#endif
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
#if __cplusplus >= 201103L
_M_insert_equal_(const_iterator __position, _Arg&& __v)
#else
_M_insert_equal_(const_iterator __position, const _Val& __v)
#endif
{
  pair<_Base_ptr, _Base_ptr> __res
= _M_get_insert_hint_equal_pos(__position, _KeyOfValue()(__v));

  if (__res.second)
return _M_insert_(__res.first, __res.second,
          _GLIBCXX_FORWARD(_Arg, __v));

  return _M_insert_equal_lower(_GLIBCXX_FORWARD(_Arg, __v));
}

#if __cplusplus >= 201103L
template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_insert_node(_Base_ptr __x, _Base_ptr __p, _Link_type __z)
{
  bool __insert_left = (__x != 0 || __p == _M_end()
            || _M_impl._M_key_compare(_S_key(__z),
                          _S_key(__p)));

  _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,
                this->_M_impl._M_header);
  ++_M_impl._M_node_count;
  return iterator(__z);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_insert_lower_node(_Base_ptr __p, _Link_type __z)
{
  bool __insert_left = (__p == _M_end()
            || !_M_impl._M_key_compare(_S_key(__p),
                           _S_key(__z)));

  _Rb_tree_insert_and_rebalance(__insert_left, __z, __p,
                this->_M_impl._M_header);
  ++_M_impl._M_node_count;
  return iterator(__z);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_insert_equal_lower_node(_Link_type __z)
{
  _Link_type __x = _M_begin();
  _Link_type __y = _M_end();
  while (__x != 0)
{
  __y = __x;
  __x = !_M_impl._M_key_compare(_S_key(__x), _S_key(__z)) ?
        _S_left(__x) : _S_right(__x);
}
  return _M_insert_lower_node(__y, __z);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
template<typename... _Args>
  pair<typename _Rb_tree<_Key, _Val, _KeyOfValue,
             _Compare, _Alloc>::iterator, bool>
  _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
  _M_emplace_unique(_Args&&... __args)
  {
_Link_type __z = _M_create_node(std::forward<_Args>(__args)...);

__try
  {
    typedef pair<iterator, bool> _Res;
    auto __res = _M_get_insert_unique_pos(_S_key(__z));
    if (__res.second)
      return _Res(_M_insert_node(__res.first, __res.second, __z), true);

    _M_destroy_node(__z);
    return _Res(iterator(static_cast<_Link_type>(__res.first)), false);
  }
__catch(...)
  {
    _M_destroy_node(__z);
    __throw_exception_again;
  }
  }

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
template<typename... _Args>
  typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
  _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
  _M_emplace_equal(_Args&&... __args)
  {
_Link_type __z = _M_create_node(std::forward<_Args>(__args)...);

__try
  {
    auto __res = _M_get_insert_equal_pos(_S_key(__z));
    return _M_insert_node(__res.first, __res.second, __z);
  }
__catch(...)
  {
    _M_destroy_node(__z);
    __throw_exception_again;
  }
  }

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
template<typename... _Args>
  typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
  _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
  _M_emplace_hint_unique(const_iterator __pos, _Args&&... __args)
  {
_Link_type __z = _M_create_node(std::forward<_Args>(__args)...);

__try
  {
    auto __res = _M_get_insert_hint_unique_pos(__pos, _S_key(__z));

    if (__res.second)
      return _M_insert_node(__res.first, __res.second, __z);

    _M_destroy_node(__z);
    return iterator(static_cast<_Link_type>(__res.first));
  }
__catch(...)
  {
    _M_destroy_node(__z);
    __throw_exception_again;
  }
  }

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
template<typename... _Args>
  typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::iterator
  _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
  _M_emplace_hint_equal(const_iterator __pos, _Args&&... __args)
  {
_Link_type __z = _M_create_node(std::forward<_Args>(__args)...);

__try
  {
    auto __res = _M_get_insert_hint_equal_pos(__pos, _S_key(__z));

    if (__res.second)
      return _M_insert_node(__res.first, __res.second, __z);

    return _M_insert_equal_lower_node(__z);
  }
__catch(...)
  {
    _M_destroy_node(__z);
    __throw_exception_again;
  }
  }
#endif

template<typename _Key, typename _Val, typename _KoV,
       typename _Cmp, typename _Alloc>
template<class _II>
  void
  _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::
  _M_insert_unique(_II __first, _II __last)
  {
for (; __first != __last; ++__first)
  _M_insert_unique_(end(), *__first);
  }

template<typename _Key, typename _Val, typename _KoV,
       typename _Cmp, typename _Alloc>
template<class _II>
  void
  _Rb_tree<_Key, _Val, _KoV, _Cmp, _Alloc>::
  _M_insert_equal(_II __first, _II __last)
  {
for (; __first != __last; ++__first)
  _M_insert_equal_(end(), *__first);
  }

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
void
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_erase_aux(const_iterator __position)
{
  _Link_type __y =
static_cast<_Link_type>(_Rb_tree_rebalance_for_erase
            (const_cast<_Base_ptr>(__position._M_node),
             this->_M_impl._M_header));
  _M_destroy_node(__y);
  --_M_impl._M_node_count;
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
void
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_erase_aux(const_iterator __first, const_iterator __last)
{
  if (__first == begin() && __last == end())
clear();
  else
while (__first != __last)
  erase(__first++);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
erase(const _Key& __x)
{
  pair<iterator, iterator> __p = equal_range(__x);
  const size_type __old_size = size();
  erase(__p.first, __p.second);
  return __old_size - size();
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
void
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
erase(const _Key* __first, const _Key* __last)
{
  while (__first != __last)
erase(*__first++);
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue,
          _Compare, _Alloc>::iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
find(const _Key& __k)
{
  iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
  return (__j == end()
      || _M_impl._M_key_compare(__k,
                _S_key(__j._M_node))) ? end() : __j;
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue,
          _Compare, _Alloc>::const_iterator
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
find(const _Key& __k) const
{
  const_iterator __j = _M_lower_bound(_M_begin(), _M_end(), __k);
  return (__j == end()
      || _M_impl._M_key_compare(__k,
                _S_key(__j._M_node))) ? end() : __j;
}

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
typename _Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::size_type
_Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
count(const _Key& __k) const
{
  pair<const_iterator, const_iterator> __p = equal_range(__k);
  const size_type __n = std::distance(__p.first, __p.second);
  return __n;
}

_GLIBCXX_PURE unsigned int
_Rb_tree_black_count(const _Rb_tree_node_base* __node,
                   const _Rb_tree_node_base* __root) throw ();

template<typename _Key, typename _Val, typename _KeyOfValue,
       typename _Compare, typename _Alloc>
bool
_Rb_tree<_Key,_Val,_KeyOfValue,_Compare,_Alloc>::__rb_verify() const
{
  if (_M_impl._M_node_count == 0 || begin() == end())
return _M_impl._M_node_count == 0 && begin() == end()
       && this->_M_impl._M_header._M_left == _M_end()
       && this->_M_impl._M_header._M_right == _M_end();

  unsigned int __len = _Rb_tree_black_count(_M_leftmost(), _M_root());
  for (const_iterator __it = begin(); __it != end(); ++__it)
{
  _Const_Link_type __x = static_cast<_Const_Link_type>(__it._M_node);
  _Const_Link_type __L = _S_left(__x);
  _Const_Link_type __R = _S_right(__x);

  if (__x->_M_color == _S_red)
    if ((__L && __L->_M_color == _S_red)
    || (__R && __R->_M_color == _S_red))
      return false;

  if (__L && _M_impl._M_key_compare(_S_key(__x), _S_key(__L)))
    return false;
  if (__R && _M_impl._M_key_compare(_S_key(__R), _S_key(__x)))
    return false;

  if (!__L && !__R && _Rb_tree_black_count(__x, _M_root()) != __len)
    return false;
}

  if (_M_leftmost() != _Rb_tree_node_base::_S_minimum(_M_root()))
return false;
  if (_M_rightmost() != _Rb_tree_node_base::_S_maximum(_M_root()))
return false;
  return true;
}



} // namespace detail


} // namespace nestl


#endif /* NESTL_DETAIL_RED_BLACK_TREE_HPP */
