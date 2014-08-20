#ifndef NESTL_ITERATOR_HPP
#define NESTL_ITERATOR_HPP

#include <nestl/config.hpp>
#include <nestl/noncopyable.hpp>
#include <nestl/class_traits.hpp>

#include <iterator>

namespace nestl
{

template <typename Container>
class back_insert_iterator
{
public:

    typedef typename Container::operation_error operation_error;
    typedef typename Container::value_type value_type;

    explicit back_insert_iterator(Container& container) noexcept
        : m_container(std::addressof(container))
    {
    }

    operation_error assign(const value_type& val) noexcept
    {
        return m_container->push_back(val);
    }

    operation_error assign(value_type&& val) noexcept
    {
        return m_container->push_back(std::move(val));
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
        return dest.assign(std::forward<Y>(src));
    }
};


template <typename Container>
class insert_iterator
{
public:

    typedef typename Container::operation_error operation_error;
    typedef typename Container::value_type value_type;
    typedef typename Container::iterator container_iterator;

    insert_iterator(Container& container, container_iterator pos) noexcept
        : m_container(std::addressof(container))
        , m_pos(pos)
    {
    }

    operation_error assign(const value_type& val) noexcept
    {
        auto err = m_container->insert(m_pos, val);
        if (!err)
        {
            m_pos = err.value();
            ++m_pos;

        }
        return err.error();
    }

    operation_error assign(value_type&& val) noexcept
    {
        auto err = m_container->insert(m_pos, std::move(val));
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
        return dest.assign(std::forward<Y>(src));
    }
};

} // namespace nestl

#endif /* NESTL_ITERATOR_HPP */
