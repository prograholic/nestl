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
#include <nestl/allocator_traits_helper.hpp>
#include <nestl/system_error.hpp>
#include <nestl/operation_error.hpp>
#include <nestl/pair.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/algorithm.hpp>
#include <nestl/swap.hpp>



namespace nestl
{

namespace detail
{

enum rb_tree_color
{
    rb_red = false,
    rb_black = true
};

struct rb_tree_node_base
{
    typedef rb_tree_node_base*       base_ptr;
    typedef const rb_tree_node_base* const_base_ptr;

    rb_tree_color m_color;
    base_ptr m_parent;
    base_ptr m_left;
    base_ptr m_right;

    static base_ptr s_minimum(base_ptr x) noexcept
    {
        NESTL_ASSERT(x);
        while (x->m_left != 0)
        {
            x = x->m_left;
        }
        return x;
    }


    static const_base_ptr s_minimum(const_base_ptr x) noexcept
    {
        NESTL_ASSERT(x);
        while (x->m_left != 0)
        {
            x = x->m_left;
        }
        return x;
    }

    static base_ptr s_maximum(base_ptr x) noexcept
    {
        NESTL_ASSERT(x);
        while (x->m_right != 0)
        {
            x = x->m_right;
        }
        return x;
    }

    static const_base_ptr s_maximum(const_base_ptr x) noexcept
    {
        while (x->m_right != 0)
        {
            x = x->m_right;
        }
        return x;
    }
};


template<typename Val>
struct rb_tree_node : public rb_tree_node_base
{
    typedef rb_tree_node<Val>* link_type;

    nestl::aligned_buffer<Val> m_storage;

    Val* m_valptr() noexcept
    {
        return m_storage.ptr();
    }

    const Val* m_valptr() const noexcept
    {
        return m_storage.ptr();
    }
};

} // namespace detail

template <typename Val>
struct class_traits<detail::rb_tree_node<Val> >
{
    typedef detail::rb_tree_node<Val> link_type;

    template <typename OperationError, typename Allocator, typename ... Args>
    static OperationError construct(link_type* ptr, Allocator& alloc, Args&& ... args) noexcept
    {
        return nestl::detail::construct<OperationError>(ptr->m_valptr(), alloc, nestl::forward<Args>(args) ...);
    }
};


namespace detail
{

template <typename RbTreeNodeBasePtr>
RbTreeNodeBasePtr rb_tree_increment(RbTreeNodeBasePtr x) noexcept
{
    if (x->m_right != 0)
    {
        x = x->m_right;
        while (x->m_left != 0)
        {
            x = x->m_left;
        }
    }
    else
    {
        RbTreeNodeBasePtr y = x->m_parent;
        while (x == y->m_right)
        {
            x = y;
            y = y->m_parent;
        }
        if (x->m_right != y)
        {
            x = y;
        }
    }
    return x;
}

template <typename RbTreeNodeBasePtr>
RbTreeNodeBasePtr rb_tree_decrement(RbTreeNodeBasePtr x) noexcept
{
    if (x->m_color == rb_red && x->m_parent->m_parent == x)
        x = x->m_right;
    else if (x->m_left != 0)
    {
        RbTreeNodeBasePtr y = x->m_left;
        while (y->m_right != 0)
            y = y->m_right;
        x = y;
    }
    else
    {
        RbTreeNodeBasePtr y = x->m_parent;
        while (x == y->m_left)
        {
            x = y;
            y = y->m_parent;
        }
        x = y;
    }
    return x;
}


template <typename T, typename OperationError>
struct rb_tree_iterator
{
    typedef nestl::ptrdiff_t                  difference_type;
    typedef nestl::bidirectional_iterator_tag iterator_category;
    typedef T                                 value_type;
    typedef T&                                reference;
    typedef T*                                pointer;
    typedef OperationError                    operation_error;



    typedef rb_tree_node_base*                base_ptr;
    typedef rb_tree_node<T>*                  link_type;

    rb_tree_iterator() noexcept
        : m_node()
    {
    }

    explicit rb_tree_iterator(link_type x) noexcept
        : m_node(x)
    {
    }

    reference operator*() const noexcept
    {
        return *static_cast<link_type>(m_node)->m_valptr();
    }

    pointer operator->() const noexcept
    {
        return static_cast<link_type> (m_node)->m_valptr();
    }

    rb_tree_iterator&
    operator++() noexcept
    {
        m_node = rb_tree_increment(m_node);
        return *this;
    }

    rb_tree_iterator
    operator++(int) noexcept
    {
        rb_tree_iterator tmp = *this;
        m_node = rb_tree_increment(m_node);
        return tmp;
    }

    rb_tree_iterator&
    operator--() noexcept
    {
        m_node = rb_tree_decrement(m_node);
        return *this;
    }

    rb_tree_iterator
    operator--(int) noexcept
    {
        rb_tree_iterator tmp = *this;
        m_node = rb_tree_decrement(m_node);
        return tmp;
    }

    bool
    operator==(const rb_tree_iterator& x) const noexcept
    {
        return m_node == x.m_node;
    }

    bool
    operator!=(const rb_tree_iterator& x) const noexcept
    {
        return m_node != x.m_node;
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

    rb_tree_const_iterator() noexcept
    : m_node() { }

    explicit
    rb_tree_const_iterator(link_type x) noexcept
    : m_node(x) { }

    rb_tree_const_iterator(const iterator& it) noexcept
    : m_node(it.m_node) { }

    iterator
    m_const_cast() const noexcept
    {
        return iterator(static_cast<typename iterator::link_type>(
                            const_cast<typename iterator::base_ptr>(m_node)));
    }

    reference
    operator*() const noexcept
    {
        return *static_cast<link_type>(m_node)->m_valptr();
    }

    pointer
    operator->() const noexcept
    {
        return static_cast<link_type>(m_node)->m_valptr();
    }

    rb_tree_const_iterator&
    operator++() noexcept
    {
        m_node = rb_tree_increment(m_node);
        return *this;
    }

    rb_tree_const_iterator
    operator++(int) noexcept
    {
        rb_tree_const_iterator tmp = *this;
        m_node = rb_tree_increment(m_node);
        return tmp;
    }

    rb_tree_const_iterator&
    operator--() noexcept
    {
        m_node = rb_tree_decrement(m_node);
        return *this;
    }

    rb_tree_const_iterator
    operator--(int) noexcept
    {
        rb_tree_const_iterator tmp = *this;
        m_node = rb_tree_decrement(m_node);
        return tmp;
    }

    bool
    operator==(const rb_tree_const_iterator& x) const noexcept
    {
        return m_node == x.m_node;
    }

    bool
    operator!=(const rb_tree_const_iterator& x) const noexcept
    {
        return m_node != x.m_node;
    }

    base_ptr m_node;
  };

template<typename Val, typename OperationError>
inline bool
operator==(const rb_tree_iterator<Val, OperationError>& x,
           const rb_tree_const_iterator<Val, OperationError>& y) noexcept
{
    return x.m_node == y.m_node;
}

template<typename Val, typename OperationError>
inline bool
operator!=(const rb_tree_iterator<Val, OperationError>& x,
           const rb_tree_const_iterator<Val, OperationError>& y) noexcept
{
    return x.m_node != y.m_node;
}


inline void local_rb_tree_rotate_left(rb_tree_node_base* const x, rb_tree_node_base*& root)
{
    rb_tree_node_base* const y = x->m_right;

    x->m_right = y->m_left;
    if (y->m_left !=0)
        y->m_left->m_parent = x;
    y->m_parent = x->m_parent;

    if (x == root)
        root = y;
    else if (x == x->m_parent->m_left)
        x->m_parent->m_left = y;
    else
        x->m_parent->m_right = y;
    y->m_left = x;
    x->m_parent = y;
}

inline void local_rb_tree_rotate_right(rb_tree_node_base* const x, rb_tree_node_base*& root)
{
    rb_tree_node_base* const y = x->m_left;

    x->m_left = y->m_right;
    if (y->m_right != 0)
        y->m_right->m_parent = x;
    y->m_parent = x->m_parent;

    if (x == root)
        root = y;
    else if (x == x->m_parent->m_right)
        x->m_parent->m_right = y;
    else
        x->m_parent->m_left = y;
    y->m_right = x;
    x->m_parent = y;
}


inline void rb_tree_insert_and_rebalance(const bool insert_left,
                                         rb_tree_node_base* x,
                                         rb_tree_node_base* p,
                                         rb_tree_node_base& header) noexcept
{
    rb_tree_node_base *& root = header.m_parent;

    // Initialize fields in new node to insert.
    x->m_parent = p;
    x->m_left = 0;
    x->m_right = 0;
    x->m_color = rb_red;

    // Insert.
    // Make new node child of parent and maintain root, leftmost and
    // rightmost nodes.
    // N.B. First node is always inserted left.
    if (insert_left)
    {
        p->m_left = x; // also makes leftmost = x when p == &header

        if (p == &header)
        {
            header.m_parent = x;
            header.m_right = x;
        }
        else if (p == header.m_left)
            header.m_left = x; // maintain leftmost pointing to min node
    }
    else
    {
        p->m_right = x;

        if (p == header.m_right)
            header.m_right = x; // maintain rightmost pointing to max node
    }
    // Rebalance.
    while (x != root && x->m_parent->m_color == rb_red)
    {
        rb_tree_node_base* const xpp = x->m_parent->m_parent;

        if (x->m_parent == xpp->m_left)
        {
            rb_tree_node_base* const y = xpp->m_right;
            if (y && y->m_color == rb_red)
            {
                x->m_parent->m_color = rb_black;
                y->m_color = rb_black;
                xpp->m_color = rb_red;
                x = xpp;
            }
            else
            {
                if (x == x->m_parent->m_right)
                {
                    x = x->m_parent;
                    local_rb_tree_rotate_left(x, root);
                }
                x->m_parent->m_color = rb_black;
                xpp->m_color = rb_red;
                local_rb_tree_rotate_right(xpp, root);
            }
        }
        else
        {
            rb_tree_node_base* const y = xpp->m_left;
            if (y && y->m_color == rb_red)
            {
                x->m_parent->m_color = rb_black;
                y->m_color = rb_black;
                xpp->m_color = rb_red;
                x = xpp;
            }
            else
            {
                if (x == x->m_parent->m_left)
                {
                    x = x->m_parent;
                    local_rb_tree_rotate_right(x, root);
                }
                x->m_parent->m_color = rb_black;
                xpp->m_color = rb_red;
                local_rb_tree_rotate_left(xpp, root);
            }
        }
    }
    root->m_color = rb_black;
}

inline rb_tree_node_base*
rb_tree_rebalance_for_erase(rb_tree_node_base* const z,
                            rb_tree_node_base& header) noexcept
{
    rb_tree_node_base *& root = header.m_parent;
    rb_tree_node_base *& leftmost = header.m_left;
    rb_tree_node_base *& rightmost = header.m_right;
    rb_tree_node_base* y = z;
    rb_tree_node_base* x = 0;
    rb_tree_node_base* x_parent = 0;

    if (y->m_left == 0)     // z has at most one non-null child. y == z.
        x = y->m_right;     // x might be null.
    else
        if (y->m_right == 0)  // z has exactly one non-null child. y == z.
            x = y->m_left;    // x is not null.
        else
        {
            // z has two non-null children.  Set y to
            y = y->m_right;   //   z's successor.  x might be null.
            while (y->m_left != 0)
                y = y->m_left;
            x = y->m_right;
        }
    if (y != z)
    {
        // relink y in place of z.  y is z's successor
        z->m_left->m_parent = y;
        y->m_left = z->m_left;
        if (y != z->m_right)
        {
            x_parent = y->m_parent;
            if (x) x->m_parent = y->m_parent;
            y->m_parent->m_left = x;   // y must be a child of m_left
            y->m_right = z->m_right;
            z->m_right->m_parent = y;
        }
        else
            x_parent = y;
        if (root == z)
            root = y;
        else if (z->m_parent->m_left == z)
            z->m_parent->m_left = y;
        else
            z->m_parent->m_right = y;
        y->m_parent = z->m_parent;
        nestl::swap(y->m_color, z->m_color);
        y = z;
        // y now points to node to be actually deleted
    }
    else
    {                        // y == z
        x_parent = y->m_parent;
        if (x)
            x->m_parent = y->m_parent;
        if (root == z)
            root = x;
        else
            if (z->m_parent->m_left == z)
                z->m_parent->m_left = x;
            else
                z->m_parent->m_right = x;
        if (leftmost == z)
        {
            if (z->m_right == 0)        // z->m_left must be null also
                leftmost = z->m_parent;
            // makes leftmost == _M_header if z == root
            else
                leftmost = rb_tree_node_base::s_minimum(x);
        }
        if (rightmost == z)
        {
            if (z->m_left == 0)         // z->m_right must be null also
                rightmost = z->m_parent;
            // makes rightmost == _M_header if z == root
            else                      // x == z->m_left
                rightmost = rb_tree_node_base::s_maximum(x);
        }
    }
    if (y->m_color != rb_red)
      {
    while (x != root && (x == 0 || x->m_color == rb_black))
      if (x == x_parent->m_left)
        {
          rb_tree_node_base* w = x_parent->m_right;
          if (w->m_color == rb_red)
        {
          w->m_color = rb_black;
          x_parent->m_color = rb_red;
          local_rb_tree_rotate_left(x_parent, root);
          w = x_parent->m_right;
        }
          if ((w->m_left == 0 ||
           w->m_left->m_color == rb_black) &&
          (w->m_right == 0 ||
           w->m_right->m_color == rb_black))
        {
          w->m_color = rb_red;
          x = x_parent;
          x_parent = x_parent->m_parent;
        }
          else
        {
          if (w->m_right == 0
              || w->m_right->m_color == rb_black)
            {
              w->m_left->m_color = rb_black;
              w->m_color = rb_red;
              local_rb_tree_rotate_right(w, root);
              w = x_parent->m_right;
            }
          w->m_color = x_parent->m_color;
          x_parent->m_color = rb_black;
          if (w->m_right)
            w->m_right->m_color = rb_black;
          local_rb_tree_rotate_left(x_parent, root);
          break;
        }
        }
      else
        {
          // same as above, with m_right <-> m_left.
          rb_tree_node_base* w = x_parent->m_left;
          if (w->m_color == rb_red)
        {
          w->m_color = rb_black;
          x_parent->m_color = rb_red;
          local_rb_tree_rotate_right(x_parent, root);
          w = x_parent->m_left;
        }
          if ((w->m_right == 0 ||
           w->m_right->m_color == rb_black) &&
          (w->m_left == 0 ||
           w->m_left->m_color == rb_black))
        {
          w->m_color = rb_red;
          x = x_parent;
          x_parent = x_parent->m_parent;
        }
          else
        {
          if (w->m_left == 0 || w->m_left->m_color == rb_black)
            {
              w->m_right->m_color = rb_black;
              w->m_color = rb_red;
              local_rb_tree_rotate_left(w, root);
              w = x_parent->m_left;
            }
          w->m_color = x_parent->m_color;
          x_parent->m_color = rb_black;
          if (w->m_left)
            w->m_left->m_color = rb_black;
          local_rb_tree_rotate_right(x_parent, root);
          break;
        }
        }
    if (x) x->m_color = rb_black;
      }
    return y;
  }


#if NESTL_HAS_RVALUE_REF

#   define NESTL_RB_TEMPLATE_ARG(Type) template <typename Type>
#   define NESTL_RB_RV_ARG(TypeIfSupported, TypeIfNotSupported) TypeIfSupported&&
#   define NESTL_RB_FORWARD(Type, arg) nestl::forward<Type>(arg)

#else /* NESTL_HAS_RVALUE_REF */

#   define NESTL_RB_TEMPLATE_ARG(Type)
#   define NESTL_RB_RV_ARG(TypeIfSupported, TypeIfNotSupported) const TypeIfNotSupported&
#   define NESTL_RB_FORWARD(Type, arg) arg

#endif /* NESTL_HAS_RVALUE_REF */


template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc = allocator<Val> >
class rb_tree
{
    typedef typename nestl::allocator_rebind<Alloc, rb_tree_node<Val> >::other node_allocator;
    typedef nestl::allocator_traits<node_allocator>            node_allocator_traits;

protected:
    typedef rb_tree_node_base* 		 base_ptr;
    typedef const rb_tree_node_base* const_base_ptr;

public:
    typedef Key                      key_type;
    typedef Val                      value_type;
    typedef value_type*              pointer;
    typedef const value_type*        const_pointer;
    typedef value_type&              reference;
    typedef const value_type&        const_reference;
    typedef rb_tree_node<Val>*       link_type;
    typedef const rb_tree_node<Val>* const_link_type;
    typedef size_t 				     size_type;
    typedef ptrdiff_t 			     difference_type;
    typedef Alloc 				     allocator_type;
    typedef OperationError           operation_error;

    typedef rb_tree_iterator<value_type, operation_error>       iterator;
    typedef rb_tree_const_iterator<value_type, operation_error> const_iterator;

    typedef nestl::reverse_iterator<iterator>       reverse_iterator;
    typedef nestl::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef pair<iterator, bool>     iterator_with_flag;

    node_allocator&
    m_get_node_allocator() noexcept
    {
        return *static_cast<node_allocator*>(&this->m_impl);
    }

    const node_allocator&
    m_get_node_allocator() const noexcept
    {
        return *static_cast<const node_allocator*>(&this->m_impl);
    }

    allocator_type
    get_allocator() const noexcept
    {
        return allocator_type(m_get_node_allocator());
    }

protected:
    link_type
    m_get_node()
    {
        return node_allocator_traits::allocate(m_get_node_allocator(), 1);
    }

    void
    m_put_node(link_type p) noexcept
    {
        node_allocator_traits::deallocate(m_get_node_allocator(), p, 1);
    }


    template<typename... Args>
    link_type m_create_node_nothrow(operation_error& error, Args&&... args) noexcept
    {
        node_allocator& node_alloc = m_get_node_allocator();
        link_type l = node_allocator_traits::allocate(node_alloc, 1);
        if (!l)
        {
            error = make_operation_error<operation_error>(nestl::errc::not_enough_memory);
            return nullptr;
        }

        nestl::detail::allocation_scoped_guard<link_type, node_allocator> guard(node_alloc, l, 1);

        error = nestl::detail::construct<operation_error>(l, node_alloc, nestl::forward<Args>(args)...);
        if (error)
        {
            return nullptr;
        }

        guard.release();
        return l;
    }

    void
    m_destroy_node(link_type p) noexcept
    {
        node_allocator_traits::destroy(m_get_node_allocator(), p->m_valptr());
        p->~rb_tree_node<Val>();
        m_put_node(p);
    }


    link_type
    m_clone_node_nothrow(operation_error& error, const_link_type x) noexcept
    {
        link_type node = m_create_node_nothrow(error, *x->m_valptr());
        if (error)
        {
            return nullptr;
        }

        node->m_color = x->m_color;
        node->m_left = 0;
        node->m_right = 0;

        return node;
    }


protected:

    template<typename KeyCompare
             /*,
             bool _Is_pod_comparator = nestl::is_pod(KeyCompare)*/>
    struct rb_tree_impl : public node_allocator
    {
        KeyCompare		m_key_compare;
        rb_tree_node_base 	m_header;
        size_type 		m_node_count; // Keeps track of size of tree.

        rb_tree_impl()
            : node_allocator()
            , m_key_compare()
            , m_header()
            , m_node_count(0)
        {
            m_initialize();
        }

        rb_tree_impl(const KeyCompare& comp, const node_allocator& a)
            : node_allocator(a)
            , m_key_compare(comp)
            , m_header()
            , m_node_count(0)
        {
            m_initialize();
        }

        rb_tree_impl(const KeyCompare& comp, node_allocator&& a)
            : node_allocator(nestl::move(a))
            , m_key_compare(comp)
            , m_header()
            , m_node_count(0)
        {
            m_initialize();
        }

    private:

        void
        m_initialize()
        {
            this->m_header.m_color = rb_red;
            this->m_header.m_parent = 0;
            this->m_header.m_left = &this->m_header;
            this->m_header.m_right = &this->m_header;
        }
    };

    rb_tree_impl<Compare> m_impl;

protected:
    base_ptr&
    m_root() noexcept
    {
        return this->m_impl.m_header.m_parent;
    }

    const_base_ptr
    m_root() const noexcept
    {
        return this->m_impl.m_header.m_parent;
    }

    base_ptr&
    m_leftmost() noexcept
    {
        return this->m_impl.m_header.m_left;
    }

    const_base_ptr
    m_leftmost() const noexcept
    {
        return this->m_impl.m_header.m_left;
    }

    base_ptr&
    m_rightmost() noexcept
    {
        return this->m_impl.m_header.m_right;
    }

    const_base_ptr
    m_rightmost() const noexcept
    {
        return this->m_impl.m_header.m_right;
    }

    link_type
    m_begin() noexcept
    {
        return static_cast<link_type>(this->m_impl.m_header.m_parent);
    }

    const_link_type
    m_begin() const noexcept
    {
        return static_cast<const_link_type>(this->m_impl.m_header.m_parent);
    }

    link_type
    m_end() noexcept
    {
        return reinterpret_cast<link_type>(&this->m_impl.m_header);
    }

    const_link_type
    m_end() const noexcept
    {
        return reinterpret_cast<const_link_type>(&this->m_impl.m_header);
    }

    static const_reference
    s_value(const_link_type x) noexcept
    {
        return *x->m_valptr();
    }

    static const Key&
    s_key(const_link_type x) noexcept
    {
        return KeyOfValue()(s_value(x));
    }

    static link_type
    s_left(base_ptr x) noexcept
    {
        return static_cast<link_type>(x->m_left);
    }

    static const_link_type
    s_left(const_base_ptr x) noexcept
    {
        return static_cast<const_link_type>(x->m_left);
    }

    static link_type
    s_right(base_ptr x) noexcept
    {
        return static_cast<link_type>(x->m_right);
    }

    static const_link_type
    s_right(const_base_ptr x) noexcept
    {
        return static_cast<const_link_type>(x->m_right);
    }

    static const_reference
    s_value(const_base_ptr x) noexcept
    {
        return *static_cast<const_link_type>(x)->m_valptr();
    }

    static const Key&
    s_key(const_base_ptr x) noexcept
    {
        return KeyOfValue()(s_value(x));
    }

    static base_ptr
    s_minimum(base_ptr x) noexcept
    {
        return rb_tree_node_base::s_minimum(x);
    }

    static const_base_ptr
    s_minimum(const_base_ptr x) noexcept
    {
        return rb_tree_node_base::s_minimum(x);
    }

    static base_ptr
    s_maximum(base_ptr x) noexcept
    {
        return rb_tree_node_base::s_maximum(x);
    }

    static const_base_ptr
    s_maximum(const_base_ptr x) noexcept
    {
        return rb_tree_node_base::s_maximum(x);
    }

private:
    pair<base_ptr, base_ptr>
    m_get_insert_unique_pos(const key_type& k) noexcept;

    pair<base_ptr, base_ptr>
    m_get_insert_equal_pos(const key_type& k) noexcept;

    pair<base_ptr, base_ptr>
    m_get_insert_hint_unique_pos(const_iterator pos, const key_type& k) noexcept;

    pair<base_ptr, base_ptr>
    m_get_insert_hint_equal_pos(const_iterator pos, const key_type& k) noexcept;

    template <typename Arg>
    iterator
    m_insert_nothrow(operation_error& error, base_ptr x, base_ptr y, Arg&& v) noexcept;

    iterator
    m_insert_node(base_ptr x, base_ptr y, link_type z) noexcept;

    template <typename Arg>
    iterator
    m_insert_lower_nothrow(operation_error& error, base_ptr y, Arg&& v) noexcept;

    template <typename Arg>
    iterator
    m_insert_equal_lower_nothrow(operation_error& error, Arg&& x) noexcept;

    iterator
    m_insert_lower_node(base_ptr p, link_type z) noexcept;

    iterator
    m_insert_equal_lower_node(link_type z) noexcept;

    link_type
    m_copy_nothrow(operation_error& error, const_link_type x, link_type p) noexcept;

    void
    m_erase(link_type x) noexcept;

    iterator
    m_lower_bound(link_type x, link_type y,
           const Key& k) noexcept;

    const_iterator
    m_lower_bound(const_link_type x, const_link_type y,
           const Key& k) const noexcept;

    iterator
    m_upper_bound(link_type x, link_type y,
           const Key& k) noexcept;

    const_iterator
    m_upper_bound(const_link_type x, const_link_type y,
           const Key& k) const noexcept;

public:
    // allocation/deallocation
    rb_tree() noexcept
        : m_impl()
    {
    }

    rb_tree(const Compare& comp, const allocator_type& a = allocator_type()) noexcept
        : m_impl(comp, node_allocator(a))
    {
    }

    rb_tree(const allocator_type& a) noexcept
    : m_impl(Compare(), node_allocator(a))
    {
    }

    rb_tree(rb_tree&& x) noexcept
        : m_impl(x.m_impl.m_key_compare, x.m_get_node_allocator())
    {
        if (x.m_root() != 0)
        {
            m_move_data(x, nestl::true_type());
        }
    }

    rb_tree(rb_tree&& x, const allocator_type& a) noexcept
        /// delegating ctor
        : rb_tree(nestl::move(x), node_allocator(a))
    {
    }

    rb_tree(rb_tree&& x, node_allocator&& a) noexcept;

    ~rb_tree() noexcept
    {
        m_erase(m_begin());
    }

    void assign_copy(operation_error& error, const rb_tree& other) noexcept
    {
        if (this != &other)
        {
            // Note that _Key may be a constant type.
            clear();
            error = nestl::detail::alloc_on_copy_nothrow(m_get_node_allocator(), other.m_get_node_allocator());
            if (error)
            {
                return;
            }

#if 0
            if (node_allocator_traits::_S_propagate_on_copy_assign())
            {
                auto& __this_alloc = this->_M_get_Node_allocator();
                auto& __that_alloc = x._M_get_Node_allocator();
                if (!_Alloc_traits::_S_always_equal()
                    && __this_alloc != __that_alloc)
                {
                    std::__alloc_on_copy(__this_alloc, __that_alloc);
                }
            }
#endif //0

            error = nestl::detail::assign(m_impl.m_key_compare, other.m_impl.m_key_compare);
            if (error)
            {
                return;
            }

            if (other.m_root() != 0)
            {
                link_type newRoot = m_copy_nothrow(error, other.m_begin(), m_end());
                if (error)
                {
                    return;
                }

                m_root() = newRoot;
                m_leftmost() = s_minimum(m_root());
                m_rightmost() = s_maximum(m_root());
                m_impl.m_node_count = other.m_impl.m_node_count;
            }
        }
    }

    // Accessors.
    Compare key_comp() const noexcept
    {
        return m_impl.m_key_compare;
    }

    iterator begin() noexcept
    {
        return iterator(static_cast<link_type>(this->m_impl.m_header.m_left));
    }

    const_iterator begin() const noexcept
    {
        return const_iterator(static_cast<const_link_type>(this->m_impl.m_header.m_left));
    }

    iterator end() noexcept
    {
        return iterator(static_cast<link_type>(&this->m_impl.m_header));
    }

    const_iterator end() const noexcept
    {
        return const_iterator(static_cast<const_link_type>(&this->m_impl.m_header));
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    bool empty() const noexcept
    {
        return m_impl.m_node_count == 0;
    }

    size_type
    size() const noexcept
    { return m_impl.m_node_count; }

    size_type
    max_size() const noexcept
    { return node_allocator_traits::max_size(m_get_node_allocator()); }

    void
    swap(rb_tree& t) noexcept;

    // Insert/erase.
    template <typename Arg>
    iterator_with_flag
    m_insert_unique_nothrow(operation_error& error, Arg&& x) noexcept;

    template <typename Arg>
    iterator
    m_insert_equal(Arg&& x) noexcept;

    template <typename Arg>
    iterator
    m_insert_unique_at_nothrow(operation_error& error, const_iterator position, Arg&& x) noexcept;

    template <typename Arg>
    iterator
    m_insert_equal_at(const_iterator position, Arg&& x) noexcept;

    template<typename... Args>
    iterator_with_flag
    m_emplace_unique_nothrow(operation_error& error, Args&&... args) noexcept;

    template<typename... Args>
    iterator
    m_emplace_equal_nothrow(operation_error& error, Args&&... args) noexcept;

    template<typename... Args>
    iterator
    m_emplace_hint_unique_nothrow(operation_error& error, const_iterator pos, Args&&... args) noexcept;

    template<typename... Args>
    iterator
    m_emplace_hint_equal_nothrow(operation_error& error, const_iterator pos, Args&&... args) noexcept;


    template<typename InputIterator>
    void
    m_insert_unique_nothrow(operation_error& error, InputIterator first, InputIterator last) noexcept;

    template<typename InputIterator>
    void
    m_insert_equal(InputIterator first, InputIterator last) noexcept;

private:
    void
    m_erase_aux(const_iterator position) noexcept;

    void
    m_erase_aux(const_iterator first, const_iterator last) noexcept;

public:

    iterator
    erase(const_iterator position) noexcept
    {
        const_iterator result = position;
        ++result;
        m_erase_aux(position);
        return result.m_const_cast();
    }

    iterator
    erase(iterator position) noexcept
    {
        iterator result = position;
        ++result;
        m_erase_aux(position);
        return result;
    }

    size_type
    erase(const key_type& x) noexcept;

    iterator
    erase(const_iterator first, const_iterator last) noexcept
    {
        m_erase_aux(first, last);
        return last.m_const_cast();
    }

    void
    erase(const key_type* first, const key_type* last) noexcept;

    void
    clear() noexcept
    {
      m_erase(m_begin());
      m_leftmost() = m_end();
      m_root() = 0;
      m_rightmost() = m_end();
      m_impl.m_node_count = 0;
    }

    // Set operations.
    iterator
    find(const key_type& k) noexcept;

    const_iterator
    find(const key_type& k) const noexcept;

    size_type
    count(const key_type& k) const noexcept;

    iterator
    lower_bound(const key_type& k) noexcept
    { return m_lower_bound(m_begin(), m_end(), k); }

    const_iterator
    lower_bound(const key_type& k) const noexcept
    { return m_lower_bound(m_begin(), m_end(), k); }

    iterator
    upper_bound(const key_type& k) noexcept
    { return m_upper_bound(m_begin(), m_end(), k); }

    const_iterator
    upper_bound(const key_type& k) const noexcept
    { return m_upper_bound(m_begin(), m_end(), k); }

    pair<iterator, iterator>
    equal_range(const key_type& k) noexcept;

    pair<const_iterator, const_iterator>
    equal_range(const key_type& k) const noexcept;

    // Debugging.
    bool
    __rb_verify() const noexcept;

    bool
    m_move_assign(rb_tree& other) noexcept;

private:
    // Move elements from container with equal allocator.
    void
    m_move_data(rb_tree& other, nestl::true_type) noexcept;

    // Move elements from container with possibly non-equal allocator,
    // which might result in a copy not a move.
    void
    m_move_data(rb_tree& other, nestl::false_type) noexcept;
};

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline bool
  operator==(const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
         const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  {
    return x.size() == y.size()
       && nestl::equal(x.begin(), x.end(), y.begin());
  }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline bool
  operator<(const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
        const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  {
    return nestl::lexicographical_compare(x.begin(), x.end(),
                    y.begin(), y.end());
  }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline bool
  operator!=(const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
         const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  { return !(x == y); }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline bool
  operator>(const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
        const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  { return y < x; }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline bool
  operator<=(const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
         const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  { return !(y < x); }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline bool
  operator>=(const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
         const rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  { return !(x < y); }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
  inline void
  swap(rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& x,
   rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& y) noexcept
  { x.swap(y); }


template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
rb_tree(rb_tree&& x, node_allocator&& a) noexcept
    : m_impl(x.m_impl.m_key_compare, nestl::move(a))
{
    typedef nestl::integral_constant<bool, node_allocator_traits::s_always_equal()> eq_t;
    if (x.m_root() != 0)
    {
        m_move_data(x, eq_t());
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_move_data(rb_tree& x, nestl::true_type) noexcept
{
    m_root() = x.m_root();
    m_leftmost() = x.m_leftmost();
    m_rightmost() = x.m_rightmost();
    m_root()->m_parent = m_end();

    x.m_root() = 0;
    x.m_leftmost() = x.m_end();
    x.m_rightmost() = x.m_end();

    this->m_impl.m_node_count = x.m_impl.m_node_count;
    x.m_impl.m_node_count = 0;
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_move_data(rb_tree& x, nestl::false_type) noexcept
{
    if (m_get_node_allocator() == x.m_get_node_allocator())
    {
        m_move_data(x, nestl::true_type());
    }
    else
    {
        m_root() = m_copy(x.m_begin(), m_end());
        m_leftmost() = s_minimum(m_root());
        m_rightmost() = s_maximum(m_root());
        m_impl.m_node_count = x.m_impl.m_node_count;
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
bool
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_move_assign(rb_tree& x) noexcept
{
    m_impl.m_key_compare = x.m_impl.m_key_compare;

    if (node_allocator_traits::_S_propagate_on_move_assign()
        || node_allocator_traits::_S_always_equal()
        || m_get_node_allocator() == x.m_get_node_allocator())
    {
        clear();
        if (x.m_root() != 0)
        {
            m_move_data(x, nestl::true_type());
        }
        nestl::detail::alloc_on_move(m_get_node_allocator(), x.m_get_node_allocator());
        return true;
    }
    return false;
}


template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_nothrow(operation_error& error, base_ptr x, base_ptr p, Arg&& v) noexcept
{
    bool insert_left = (x != 0 || p == m_end()
                        || m_impl.m_key_compare(KeyOfValue()(v), s_key(p)));

    link_type z = m_create_node_nothrow(error, nestl::forward<Arg>(v));
    if (error)
    {
        return iterator{};
    }

    rb_tree_insert_and_rebalance(insert_left, z, p, this->m_impl.m_header);
    ++m_impl.m_node_count;
    
    return iterator{z};
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_lower_nothrow(operation_error& error, base_ptr p, Arg&& v) noexcept
{
    bool insert_left = (p == m_end() || !m_impl.m_key_compare(s_key(p), KeyOfValue()(v)));

    link_type z = m_create_node_nothrow(error, nestl::forward<Arg>(v));
    if (error)
    {
        return iterator{};
    }

    rb_tree_insert_and_rebalance(insert_left, z, p, this->m_impl.m_header);
    ++m_impl.m_node_count;
    
    return z;
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_equal_lower_nothrow(operation_error& error, Arg&& v) noexcept
{
    link_type x = m_begin();
    link_type y = m_end();
    while (x != 0)
    {
        y = x;
        x = !m_impl.m_key_compare(s_key(x), KeyOfValue()(v)) ? s_left(x) : s_right(x);
    }
    return m_insert_lower_nothrow(error, y, nestl::forward<Arg>(v));
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::link_type
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_copy_nothrow(operation_error& error, const_link_type x, link_type p) noexcept
{
    // Structural copy.  x and p must be non-null.
    link_type top = m_clone_node_nothrow(error, x);
    if (error)
    {
        return nullptr;
    }
    top->m_parent = p;

    class erase_guard : private nestl::noncopyable
    {
    public:

        erase_guard(rb_tree& tree, link_type t)
            : m_top(t)
            , m_tree(tree)
        {
        }

        ~erase_guard()
        {
            if (m_top)
            {
                m_tree.m_erase(m_top);
            }
        }

        void release()
        {
            m_top = 0;
        }

    private:
        link_type m_top;
        rb_tree& m_tree;
    };

    erase_guard guard(*this, top);

    if (x->m_right)
    {
        top->m_right = m_copy_nothrow(error, s_right(x), top);
        if (error)
        {
            return nullptr;
        }
    }
    p = top;
    x = s_left(x);

    while (x != 0)
    {
        link_type y = m_clone_node_nothrow(error, x);
        if (error)
        {
            return nullptr;
        }

        p->m_left = y;
        y->m_parent = p;
        if (x->m_right)
        {
            y->m_right = m_copy_nothrow(error, s_right(x), y);
            if (error)
            {
                return nullptr;
            }
        }
        p = y;
        x = s_left(x);
    }
    guard.release();
    return top;
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_erase(link_type x) noexcept
{
    // Erase without rebalancing.
    while (x != 0)
    {
        m_erase(s_right(x));
        link_type y = s_left(x);
        m_destroy_node(x);
        x = y;
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_lower_bound(link_type x, link_type y, const Key& k) noexcept
{
    while (x != 0)
    {
        if (!m_impl.m_key_compare(s_key(x), k))
        {
            y = x, x = s_left(x);
        }
        else
        {
            x = s_right(x);
        }
    }
    return iterator(y);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::const_iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_lower_bound(const_link_type x, const_link_type y, const Key& k) const noexcept
{
    while (x != 0)
    {
        if (!m_impl.m_key_compare(s_key(x), k))
        {
            y = x, x = s_left(x);
        }
        else
        {
            x = s_right(x);
        }
    }
    return const_iterator(y);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_upper_bound(link_type x, link_type y, const Key& k) noexcept
{
    while (x != 0)
    {
        if (m_impl.m_key_compare(k, s_key(x)))
        {
            y = x, x = s_left(x);
        }
        else
        {
            x = s_right(x);
        }
    }
    return iterator(y);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::const_iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_upper_bound(const_link_type x, const_link_type y, const Key& k) const noexcept
{
    while (x != 0)
    {
        if (m_impl.m_key_compare(k, s_key(x)))
        {
            y = x, x = s_left(x);
        }
        else
        {
            x = s_right(x);
        }
    }
    return const_iterator(y);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
pair<typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator,
     typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
equal_range(const Key& k) noexcept
{
    link_type x = m_begin();
    link_type y = m_end();
    while (x != 0)
    {
        if (m_impl.m_key_compare(s_key(x), k))
        {
            x = s_right(x);
        }
        else if (m_impl.m_key_compare(k, s_key(x)))
        {
            y = x, x = s_left(x);
        }
        else
        {
            link_type xu(x), yu(y);
            y = x, x = s_left(x);
            xu = s_right(xu);
            return pair<iterator, iterator>(m_lower_bound(x, y, k), m_upper_bound(xu, yu, k));
        }
    }
    return pair<iterator, iterator>(iterator(y), iterator(y));
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
pair<typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::const_iterator,
     typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::const_iterator>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
equal_range(const Key& k) const noexcept
{
    const_link_type x = m_begin();
    const_link_type y = m_end();
    while (x != 0)
    {
        if (m_impl.m_key_compare(s_key(x), k))
        {
            x = s_right(x);
        }
        else if (m_impl.m_key_compare(k, s_key(x)))
        {
            y = x, x = s_left(x);
        }
        else
        {
            const_link_type xu(x), yu(y);
            y = x, x = s_left(x);
            xu = s_right(xu);
            return pair<const_iterator, const_iterator>(m_lower_bound(x, y, k), m_upper_bound(xu, yu, k));
        }
    }
    return pair<const_iterator, const_iterator>(const_iterator(y), const_iterator(y));
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
swap(rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>& t) noexcept
{
    if (m_root() == 0)
    {
        if (t.m_root() != 0)
        {
            m_root() = t.m_root();
            m_leftmost() = t.m_leftmost();
            m_rightmost() = t.m_rightmost();
            m_root()->m_parent = m_end();

            t.m_root() = 0;
            t.m_leftmost() = t.m_end();
            t.m_rightmost() = t.m_end();
        }
    }
    else if (t.m_root() == 0)
    {
        t.m_root() = m_root();
        t.m_leftmost() = m_leftmost();
        t.m_rightmost() = m_rightmost();
        t.m_root()->m_parent = t.m_end();

        m_root() = 0;
        m_leftmost() = m_end();
        m_rightmost() = m_end();
    }
    else
    {
        nestl::swap(m_root(),t.m_root());
        nestl::swap(m_leftmost(),t.m_leftmost());
        nestl::swap(m_rightmost(),t.m_rightmost());

        m_root()->m_parent = m_end();
        t.m_root()->m_parent = t.m_end();
    }
    // No need to swap header's color as it does not change.
    nestl::swap(this->m_impl.m_node_count, t.m_impl.m_node_count);
    nestl::swap(this->m_impl.m_key_compare, t.m_impl.m_key_compare);

    node_allocator_traits::_S_on_swap(m_get_node_allocator(),
              t.m_get_node_allocator());
  }

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
pair<typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr,
     typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_get_insert_unique_pos(const key_type& k) noexcept
{
    typedef pair<base_ptr, base_ptr> result_type;
    link_type x = m_begin();
    link_type y = m_end();
    bool comp = true;
    while (x != 0)
    {
        y = x;
        comp = m_impl.m_key_compare(k, s_key(x));
        x = comp ? s_left(x) : s_right(x);
    }
    iterator j = iterator(y);
    if (comp)
    {
        if (j == begin())
        {
            return result_type(x, y);
        }
        else
        {
            --j;
        }
    }
    if (m_impl.m_key_compare(s_key(j.m_node), k))
    {
        return result_type(x, y);
    }
    return result_type(j.m_node, 0);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
pair<typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr,
     typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_get_insert_equal_pos(const key_type& k) noexcept
{
    typedef pair<base_ptr, base_ptr> result_type;
    link_type x = m_begin();
    link_type y = m_end();
    while (x != 0)
    {
        y = x;
        x = m_impl.m_key_compare(k, s_key(x)) ? s_left(x) : s_right(x);
    }
    return result_type(x, y);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator_with_flag
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_unique_nothrow(operation_error& error, Arg&& v) noexcept
{
    pair<base_ptr, base_ptr> res = m_get_insert_unique_pos(KeyOfValue()(v));

    if (res.second)
    {
        iterator insert_pos = m_insert_nothrow(error, res.first, res.second, nestl::forward<Arg>(v));
        if (error)
        {
            return iterator_with_flag{};
        }
        return iterator_with_flag(insert_pos, true);
    }

    return iterator_with_flag(iterator(static_cast<link_type>(res.first)), false);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_equal(Arg&& v) noexcept
{
    pair<base_ptr, base_ptr> res = m_get_insert_equal_pos(KeyOfValue()(v));
    return m_insert_(res.first, res.second, nestl::forward<Arg>(v));
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
pair<typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr,
     typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_get_insert_hint_unique_pos(const_iterator position, const key_type& k) noexcept
{
    iterator pos = position.m_const_cast();
    typedef pair<base_ptr, base_ptr> result_type;

    // end()
    if (pos.m_node == m_end())
    {
        if (size() > 0 && m_impl.m_key_compare(s_key(m_rightmost()), k))
        {
            return result_type(0, m_rightmost());
        }
        else
        {
            return m_get_insert_unique_pos(k);
        }
    }
    else if (m_impl.m_key_compare(k, s_key(pos.m_node)))
    {
        // First, try before...
        iterator before = pos;
        if (pos.m_node == m_leftmost()) // begin()
        {
            return result_type(m_leftmost(), m_leftmost());
        }
        else if (m_impl.m_key_compare(s_key((--before).m_node), k))
        {
            if (s_right(before.m_node) == 0)
            {
                return result_type(0, before.m_node);
            }
            else
            {
                return result_type(pos.m_node, pos.m_node);
            }
        }
        else
        {
            return m_get_insert_unique_pos(k);
        }
    }
    else if (m_impl.m_key_compare(s_key(pos.m_node), k))
    {
        // ... then try after.
        iterator after = pos;
        if (pos.m_node == m_rightmost())
        {
            return result_type(0, m_rightmost());
        }
        else if (m_impl.m_key_compare(k, s_key((++after).m_node)))
        {
            if (s_right(pos.m_node) == 0)
            {
                return result_type(0, pos.m_node);
            }
            else
            {
                return result_type(after.m_node, after.m_node);
            }
        }
        else
        {
            return m_get_insert_unique_pos(k);
        }
    }
    else
    {
        // Equivalent keys.
        return result_type(pos.m_node, 0);
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_unique_at_nothrow(operation_error& error, const_iterator position, Arg&& v) noexcept
{
    pair<base_ptr, base_ptr> res = m_get_insert_hint_unique_pos(position, KeyOfValue()(v));

    if (res.second)
    {
        return m_insert_nothrow(error, res.first, res.second, nestl::forward<Arg>(v));
    }
    return static_cast<link_type>(res.first);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
pair<typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr,
     typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::base_ptr>
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_get_insert_hint_equal_pos(const_iterator position, const key_type& k) noexcept
{
    iterator pos = position.m_const_cast();
    typedef pair<base_ptr, base_ptr> result_type;

    // end()
    if (pos.m_node == m_end())
    {
        if (size() > 0 && !m_impl.m_key_compare(k, s_key(m_rightmost())))
        {
            return result_type(0, m_rightmost());
        }
        else
        {
            return m_get_insert_equal_pos(k);
        }
    }
    else if (!m_impl.m_key_compare(s_key(pos.m_node), k))
    {
        // First, try before...
        iterator before = pos;
        if (pos.m_node == m_leftmost()) // begin()
        {
            return result_type(m_leftmost(), m_leftmost());
        }
        else if (!m_impl.m_key_compare(k, s_key((--before).m_node)))
        {
            if (s_right(before.m_node) == 0)
            {
                return result_type(0, before.m_node);
            }
            else
            {
                return result_type(pos.m_node, pos.m_node);
            }
        }
        else
        {
            return m_get_insert_equal_pos(k);
        }
    }
    else
    {
        // ... then try after.
        iterator after = pos;
        if (pos.m_node == m_rightmost())
        {
            return result_type(0, m_rightmost());
        }
        else if (!m_impl.m_key_compare(s_key((++after).m_node), k))
        {
            if (s_right(pos.m_node) == 0)
            {
                return result_type(0, pos.m_node);
            }
            else
            {
                return result_type(after.m_node, after.m_node);
            }
        }
        else
        {
            return result_type(0, 0);
        }
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template <typename Arg>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_equal_at(const_iterator position, Arg&& v) noexcept
{
    pair<base_ptr, base_ptr> res = m_get_insert_hint_equal_pos(position, KeyOfValue()(v));
    if (res.second)
    {
        return m_insert_(res.first, res.second, nestl::forward<Arg>(v));
    }

    return m_insert_equal_lower(nestl::forward<Arg>(v));
}


template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_node(base_ptr x, base_ptr p, link_type z) noexcept
{
    bool insert_left = (x != 0 || p == m_end() || m_impl.m_key_compare(s_key(z), s_key(p)));

    rb_tree_insert_and_rebalance(insert_left, z, p, this->m_impl.m_header);
    ++m_impl.m_node_count;
    return iterator(z);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_lower_node(base_ptr p, link_type z) noexcept
{
    bool insert_left = (p == m_end() || !m_impl.m_key_compare(s_key(p), s_key(z)));

    rb_tree_insert_and_rebalance(insert_left, z, p, this->m_impl.m_header);
    ++m_impl.m_node_count;
    return iterator(z);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_equal_lower_node(link_type z) noexcept
{
    link_type x = m_begin();
    link_type y = m_end();
    while (x != 0)
    {
        y = x;
        x = !m_impl.m_key_compare(s_key(x), s_key(z)) ? s_left(x) : s_right(x);
    }
    return m_insert_lower_node(y, z);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template<typename... Args>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator_with_flag
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_emplace_unique_nothrow(operation_error& error, Args&&... args) noexcept
{
    link_type z = m_create_node_nothrow(error, nestl::forward<Args>(args)...);
    if (error)
    {
        /// @todo repack
        return iterator_with_flag{};
    }

    auto res = m_get_insert_unique_pos(s_key(z));
    if (res.second)
    {
        return iterator_with_flag(m_insert_node(res.first, res.second, z), true);
    }

    m_destroy_node(z);
    return iterator_with_flag(iterator(static_cast<link_type>(res.first)), false);
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template<typename... Args>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_emplace_equal_nothrow(operation_error& error, Args&&... args) noexcept
{
    link_type z = m_create_node_nothrow(error, nestl::forward<Args>(args)...);
    if (error)
    {
        return iterator{};
    }

    auto res = m_get_insert_equal_pos(s_key(z));
    return m_insert_node(res.first, res.second, z);


#if 0
    __catch(...)
    {
        m_destroy_node(z);
        __throw_exception_again;
    }
#endif //0
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template<typename... Args>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_emplace_hint_unique_nothrow(operation_error& error, const_iterator pos, Args&&... args) noexcept
{
    link_type z = m_create_node_nothrow(error, nestl::forward<Args>(args)...);
    if (error)
    {
        return iterator{};
    }

    auto res = m_get_insert_hint_unique_pos(pos, s_key(z));

    if (res.second)
    {
        return m_insert_node(res.first, res.second, z);
    }

    m_destroy_node(z);
    return iterator(static_cast<link_type>(res.first));

#if 0
    __catch(...)
    {
        m_destroy_node(z);
        __throw_exception_again;
    }
#endif //0
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template<typename... Args>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_emplace_hint_equal_nothrow(operation_error& error, const_iterator pos, Args&&... args) noexcept
{
    link_type z = m_create_node_nothrow(error, nestl::forward<Args>(args)...);
    if (error)
    {
        return iterator{};
    }

    auto res = m_get_insert_hint_equal_pos(pos, s_key(z));
    if (res.second)
    {
        return m_insert_node(res.first, res.second, z);
    }

    return m_insert_equal_lower_node(z);

#if 0
  __catch(...)
    {
      m_destroy_node(z);
      __throw_exception_again;
    }
#endif //0
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template<class InputIterator>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_unique_nothrow(operation_error& error, InputIterator first, InputIterator last) noexcept
{
    for (; first != last; ++first)
    {
        m_insert_unique_at_nothrow(error, end(), *first);
        if (error)
        {
            return;
        }
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
template<class InputIterator>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_insert_equal(InputIterator first, InputIterator last) noexcept
{
    for (; first != last; ++first)
    {
        m_insert_equal_at(end(), *first);
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_erase_aux(const_iterator position) noexcept
{
    link_type y = static_cast<link_type>(rb_tree_rebalance_for_erase(
                                             const_cast<base_ptr>(position.m_node),
                                             this->m_impl.m_header));
    m_destroy_node(y);
    --m_impl.m_node_count;
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
m_erase_aux(const_iterator first, const_iterator last) noexcept
{
    if (first == begin() && last == end())
    {
        clear();
    }
    else
    {
        while (first != last)
        {
            erase(first++);
        }
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::size_type
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
erase(const Key& x) noexcept
{
    pair<iterator, iterator> p = equal_range(x);
    const size_type old_size = size();
    erase(p.first, p.second);
    return old_size - size();
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
void
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
erase(const Key* first, const Key* last) noexcept
{
    while (first != last)
    {
        erase(*first++);
    }
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
find(const Key& k) noexcept
{
    iterator j = m_lower_bound(m_begin(), m_end(), k);
    return (j == end() || m_impl.m_key_compare(k, s_key(j.m_node))) ? end() : j;
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::const_iterator
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
find(const Key& k) const noexcept
{
    const_iterator j = m_lower_bound(m_begin(), m_end(), k);
    return (j == end() || m_impl.m_key_compare(k, s_key(j.m_node))) ? end() : j;
}

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
typename rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::size_type
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
count(const Key& k) const noexcept
{
    pair<const_iterator, const_iterator> p = equal_range(k);
    const size_type n = nestl::distance(p.first, p.second);
    return n;
}

unsigned int
rb_tree_black_count(const rb_tree_node_base* node, const rb_tree_node_base* root) noexcept;

template<typename Key, typename Val, typename KeyOfValue,
         typename Compare, typename OperationError, typename Alloc>
bool
rb_tree<Key, Val, KeyOfValue, Compare, OperationError, Alloc>::
__rb_verify() const noexcept
{
    if (m_impl.m_node_count == 0 || begin() == end())
    {
        return m_impl.m_node_count == 0 && begin() == end()
                && this->m_impl.m_header.m_left == m_end()
                && this->m_impl.m_header.m_right == m_end();
    }

    unsigned int len = rb_tree_black_count(m_leftmost(), m_root());
    for (const_iterator it = begin(); it != end(); ++it)
    {
        const_link_type x = static_cast<const_link_type>(it.m_node);
        const_link_type left = s_left(x);
        const_link_type right = s_right(x);

        if (x->m_color == rb_red)
        {
            if ((left && left->m_color == rb_red) || (right && right->m_color == rb_red))
            {
                return false;
            }
        }

        if (left && m_impl.m_key_compare(s_key(x), s_key(left)))
        {
            return false;
        }
        if (right && m_impl.m_key_compare(s_key(right), s_key(x)))
        {
            return false;
        }

        if (!left && !right && rb_tree_black_count(x, m_root()) != len)
        {
            return false;
        }
    }

    if (m_leftmost() != rb_tree_node_base::s_minimum(m_root()))
    {
        return false;
    }
    if (m_rightmost() != rb_tree_node_base::s_maximum(m_root()))
    {
        return false;
    }

    return true;
}


} // namespace detail

} // namespace nestl


#endif /* NESTL_DETAIL_RED_BLACK_TREE_HPP */
