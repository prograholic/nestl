#ifndef NESTL_ITERATOR_HPP
#define NESTL_ITERATOR_HPP

#include <nestl/config.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/system_error.hpp>
#include <nestl/move.hpp>

namespace nestl
{


struct input_iterator_tag
{
};

struct output_iterator_tag
{
};

struct forward_iterator_tag : public input_iterator_tag
{
};

struct bidirectional_iterator_tag : public forward_iterator_tag
{
};

struct random_access_iterator_tag : public bidirectional_iterator_tag
{
};

template
<
        typename IteratorCategory,
        typename T,
        typename OperationError = nestl::error_condition,
        typename Distance = ptrdiff_t,
        typename Pointer = T*,
        typename Reference = T&
>
class iterator
{
    typedef T              value_type;
    typedef OperationError operation_error;
    typedef Distance       difference_type;
    typedef Pointer        pointer_type;
    typedef Reference      reference_type;
};


template <typename Iterator>
struct iterator_traits
{
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::operation_error   operation_error;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
    typedef typename Iterator::iterator_category iterator_category;
};

template <typename T>
struct iterator_traits<T*>
{
    typedef T                          value_type;
    typedef nestl::error_condition     operation_error;
    typedef ptrdiff_t                  difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
    typedef random_access_iterator_tag iterator_category;
};

template <typename T>
struct iterator_traits<const T*>
{
    typedef T                          value_type;
    typedef nestl::error_condition     operation_error;
    typedef ptrdiff_t                  difference_type;
    typedef const T*                   pointer;
    typedef const T&                   reference;
    typedef random_access_iterator_tag iterator_category;
};



template <typename Container>
typename Container::const_pointer begin(const Container& c)
{
    return c.begin();
}

template <typename Container>
typename Container::pointer begin(Container& c)
{
    return c.begin();
}

template <typename Container>
typename Container::const_pointer end(const Container& c)
{
    return c.end();
}

template <typename Container>
typename Container::pointer end(Container& c)
{
    return c.end();
}

template<class T, size_t N>
T* begin(T (& arr)[N])
{
    return arr;
}

template<class T, size_t N>
T* end(T (& arr)[N])
{
    return arr + N;
}


template<typename Iterator>
class reverse_iterator : public nestl::iterator<
        typename nestl::iterator_traits<Iterator>::iterator_category,
        typename nestl::iterator_traits<Iterator>::value_type,
        typename nestl::iterator_traits<Iterator>::operation_error,
        typename nestl::iterator_traits<Iterator>::difference_type,
        typename nestl::iterator_traits<Iterator>::pointer,
        typename nestl::iterator_traits<Iterator>::reference>
{
public:
    typedef nestl::iterator_traits<Iterator>      traits_type;

    typedef Iterator                              iterator_type;
    typedef typename traits_type::difference_type difference_type;
    typedef typename traits_type::pointer         pointer;
    typedef typename traits_type::reference       reference;

    reverse_iterator()
        : m_current()
    {
    }

    explicit reverse_iterator(iterator_type x)
        : m_current(x)
    {
    }

    reverse_iterator(const reverse_iterator& x)
        : m_current(x.m_current)
    {
    }

    template<typename OtherIterator>
    reverse_iterator(const reverse_iterator<OtherIterator>& x)
        : m_current(x.base())
    {
    }

    iterator_type base() const
    {
        return m_current;
    }

    reference operator*() const
    {
        Iterator tmp = m_current;
        return *--tmp;
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    reverse_iterator& operator++()
    {
        --m_current;
        return *this;
    }

    reverse_iterator operator++(int)
    {
        reverse_iterator tmp = *this;
        --m_current;
        return tmp;
    }

    reverse_iterator& operator--()
    {
        ++m_current;
        return *this;
    }

    reverse_iterator operator--(int)
    {
        reverse_iterator tmp = *this;
        ++m_current;
        return tmp;
    }

    reverse_iterator operator+(difference_type n) const
    {
        return reverse_iterator(m_current - n);
    }

    reverse_iterator& operator+=(difference_type n)
    {
        m_current -= n;
        return *this;
    }

    reverse_iterator operator-(difference_type n) const
    {
        return reverse_iterator(m_current + n);
    }

    reverse_iterator& operator-=(difference_type n)
    {
        m_current += n;
        return *this;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

private:
  Iterator m_current;
};

template<typename Iterator>
inline bool
operator==(const reverse_iterator<Iterator>& x,
           const reverse_iterator<Iterator>& y)
{
    return x.base() == y.base();
}

template<typename Iterator>
inline bool
operator<(const reverse_iterator<Iterator>& x,
          const reverse_iterator<Iterator>& y)
{
    return y.base() < x.base();
}

template<typename Iterator>
inline bool
operator!=(const reverse_iterator<Iterator>& x,
           const reverse_iterator<Iterator>& y)
{
    return !(x == y);
}

template<typename Iterator>
inline bool
operator>(const reverse_iterator<Iterator>& x,
          const reverse_iterator<Iterator>& y)
{
    return y < x;
}

template<typename Iterator>
inline bool
operator<=(const reverse_iterator<Iterator>& x,
           const reverse_iterator<Iterator>& y)
{
    return !(y < x);
}

template<typename Iterator>
inline bool
operator>=(const reverse_iterator<Iterator>& x,
           const reverse_iterator<Iterator>& y)
{
    return !(x < y);
}

template<typename Iterator>
inline typename reverse_iterator<Iterator>::difference_type
operator-(const reverse_iterator<Iterator>& x,
          const reverse_iterator<Iterator>& y)
{
    return y.base() - x.base();
}

template<typename Iterator>
inline reverse_iterator<Iterator>
operator+(typename reverse_iterator<Iterator>::difference_type n,
          const reverse_iterator<Iterator>& x)
{
    return reverse_iterator<Iterator>(x.base() - n);
}

template<typename IteratorL, typename IteratorR>
inline bool
operator==(const reverse_iterator<IteratorL>& x,
           const reverse_iterator<IteratorR>& y)
{
    return x.base() == y.base();
}

template<typename IteratorL, typename IteratorR>
inline bool
operator<(const reverse_iterator<IteratorL>& x,
          const reverse_iterator<IteratorR>& y)
{
    return y.base() < x.base();
}

template<typename IteratorL, typename IteratorR>
inline bool
operator!=(const reverse_iterator<IteratorL>& x,
           const reverse_iterator<IteratorR>& y)
{
    return !(x == y);
}

template<typename IteratorL, typename IteratorR>
inline bool
operator>(const reverse_iterator<IteratorL>& x,
          const reverse_iterator<IteratorR>& y)
{
    return y < x;
}

template<typename IteratorL, typename IteratorR>
inline bool
operator<=(const reverse_iterator<IteratorL>& x,
           const reverse_iterator<IteratorR>& y)
{
    return !(y < x);
}

template<typename IteratorL, typename IteratorR>
inline bool
operator>=(const reverse_iterator<IteratorL>& x,
           const reverse_iterator<IteratorR>& y)
{
    return !(x < y);
}

template<typename IteratorL, typename IteratorR>
inline typename reverse_iterator<IteratorL>::difference_type
operator-(const reverse_iterator<IteratorL>& x,
          const reverse_iterator<IteratorR>& y)
{
    return y.base() - x.base();
}






template <typename Container>
class back_insert_iterator : public iterator <output_iterator_tag, typename Container::operation_error, void, void, void, void>
{
public:

    typedef iterator <output_iterator_tag, typename Container::operation_error, void, void, void, void> base_type;
    typedef typename base_type::operation_error operation_error;

    typedef typename Container::value_type      container_value_type;

    explicit back_insert_iterator(Container& container) noexcept
        : m_container(nestl::addressof(container))
    {
    }

    operation_error assign(const container_value_type& val) noexcept
    {
        operation_error error;
        m_container->push_back_nothrow(error, val);
        return error;
    }

    operation_error assign(container_value_type&& val) noexcept
    {
        operation_error error;
        m_container->push_back_nothrow(error, nestl::move(val));
        return error;
    }

    back_insert_iterator& operator*() noexcept
    {
        return *this;
    }

    back_insert_iterator& operator++() noexcept
    {
        return *this;
    }

    back_insert_iterator operator++(int) noexcept
    {
        return *this;
    }

private:
    Container* m_container;
};


template <typename Container>
back_insert_iterator<Container> back_inserter(Container& c) noexcept
{
    return back_insert_iterator<Container>(c);
}

/**
 * @brief Specialization of class_traits class for using back_insert_iterator in algorithms
 */
template <typename Container>
struct class_traits<nestl::back_insert_iterator<Container> >
{

    template <typename OperationError, typename Y>
    static OperationError assign(nestl::back_insert_iterator<Container>& dest, Y&& src) noexcept
    {
        OperationError error;
        dest.assign_nothrow(error, nestl::forward<Y>(src));
        return error;
    }

    template <typename OperationError, typename Y>
    static OperationError assign(nestl::back_insert_iterator<Container>& dest, const Y& src) noexcept
    {
        OperationError error;
        dest.assign_nothrow(error, src);
        return error;
    }
};


template <typename Container>
class insert_iterator : public iterator <output_iterator_tag, typename Container::operation_error, void, void, void, void>
{
public:

    typedef iterator <output_iterator_tag, typename Container::operation_error, void, void, void, void> base_type;
    typedef typename base_type::operation_error operation_error;
    typedef typename Container::value_type      container_value_type;
    typedef typename Container::iterator        container_iterator;


    insert_iterator(Container& container, container_iterator pos) noexcept
        : m_container(nestl::addressof(container))
        , m_pos(pos)
    {
    }

    operation_error assign(const container_value_type& val) noexcept
    {
        operation_error err = m_container->insert(m_pos, val);
        if (!err)
        {
            m_pos = err.value();
            ++m_pos;

        }
        return err.error();
    }

    operation_error assign(container_value_type&& val) noexcept
    {
        operation_error err = m_container->insert(m_pos, nestl::move(val));
        if (!err)
        {
            m_pos = err.value();
            ++m_pos;

        }
        return err.error();
    }

    insert_iterator& operator*() noexcept
    {
        return *this;
    }

    insert_iterator& operator++() noexcept
    {
        return *this;
    }

    insert_iterator operator++(int) noexcept
    {
        return *this;
    }

private:
    Container* m_container;

    container_iterator m_pos;
};


template <typename Container, typename Iterator>
insert_iterator<Container> inserter(Container& c, Iterator pos) noexcept
{
    return insert_iterator<Container>(c, pos);
}

/**
 * @brief Specialization of class_traits class for using back_insert_iterator in algorithms
 */
template <typename Container>
struct class_traits<nestl::insert_iterator<Container> >
{
    template <typename OperationError, typename Y>
    static OperationError assign(nestl::insert_iterator<Container>& dest, Y&& src) noexcept
    {
        return dest.assign(nestl::forward<Y>(src));
    }
};


template <typename Iterator>
struct move_iterator
{

};


template <typename Iterator>
move_iterator<Iterator> make_move_iterator(Iterator i)
{
    return move_iterator<Iterator>(i);
}


} // namespace nestl

#endif /* NESTL_ITERATOR_HPP */
