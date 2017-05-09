#ifndef NESTL_HAS_EXCEPTIONS_LIST_HPP
#define NESTL_HAS_EXCEPTIONS_LIST_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/list.hpp>

#include <nestl/has_exceptions/default_operation_error.hpp>

namespace nestl
{
namespace has_exceptions
{

template <typename T, typename Allocator = nestl::allocator<T> >
class list : private impl::list<T, Allocator>
{
    typedef impl::list<T, Allocator> base_t;
public:

    typedef typename base_t::value_type             value_type;
    typedef typename base_t::allocator_type         allocator_type;
    typedef typename base_t::size_type              size_type;
    typedef typename base_t::difference_type        difference_type;
    typedef typename base_t::reference              reference;
    typedef typename base_t::const_reference        const_reference;
    typedef typename base_t::pointer                pointer;
    typedef typename base_t::const_pointer          const_pointer;
    typedef typename base_t::iterator               iterator;
    typedef typename base_t::const_iterator         const_iterator;
    typedef typename base_t::reverse_iterator       reverse_iterator;
    typedef typename base_t::const_reverse_iterator const_reverse_iterator;

    explicit list(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC
        : base_t(alloc)
    {
    }

    explicit list(list&& other) NESTL_NOEXCEPT_SPEC
        : base_t(std::move(other))
    {
    }

    list& operator=(list&& other) NESTL_NOEXCEPT_SPEC
    {
        return base_t::operator=(other);
    }

    list(const list& other)
    {
        default_operation_error err;
        this->copy_nothrow(err, other);
        if (err)
        {
            throw_exception(err);
        }
    }

    list& operator=(const list& other)
    {
        list tmp(other);
        this->swap(tmp);

        return *this;
    }
    // TODO: add missing ctors and methods;
    // TODO: add ctors and operators from no_exceptions::list

    using base_t::get_allocator;
    using base_t::copy_nothrow;
    using base_t::assign_nothrow;
    using base_t::front;
    using base_t::back;
    using base_t::begin;
    using base_t::cbegin;
    using base_t::end;
    using base_t::cend;
    using base_t::rbegin;
    using base_t::crbegin;
    using base_t::rend;
    using base_t::crend;
    using base_t::empty;
    using base_t::size;
    using base_t::max_size;
    using base_t::clear;
    using base_t::insert_nothrow;
    using base_t::emplace_nothrow;
    using base_t::erase;
    using base_t::push_back_nothrow;
    using base_t::emplace_back_nothrow;
    using base_t::pop_back;
    using base_t::push_front_nothrow;
    using base_t::emplace_front_nothrow;
    using base_t::pop_front;
    using base_t::resize_nothrow;
    using base_t::swap;
    using base_t::splice;
    using base_t::remove;
    using base_t::reverse;
    using base_t::unique;
    using base_t::sort;

    void push_back(const value_type& value)
    {
        default_operation_error err;
        push_back_nothrow(err, value);
        if (err)
        {
            throw_exception(err);
        }
    }

    void push_back(value_type&& value)
    {
        default_operation_error err;
        push_back_nothrow(err, std::move(value));
        if (err)
        {
            throw_exception(err);
        }
    }

    void merge(list& other) NESTL_NOEXCEPT_SPEC
    {
        base_t::merge(other);
    }

    void merge(list&& other) NESTL_NOEXCEPT_SPEC
    {
        base_t::merge(other);
    }

    template <typename Compare>
    void merge(list& other, Compare comp) NESTL_NOEXCEPT_SPEC
    {
        base_t::merge(other, comp);
    }

    template <typename Compare>
    void merge(list&& other, Compare comp) NESTL_NOEXCEPT_SPEC
    {
        base_t::merge(other, comp);
    }

    void splice(const_iterator pos, list& other) NESTL_NOEXCEPT_SPEC
    {
        base_t::splice(pos, other);
    }

    void splice(const_iterator pos, list&& other) NESTL_NOEXCEPT_SPEC
    {
        base_t::splice(pos, other);
    }

    void splice(const_iterator pos, list& other, const_iterator it) NESTL_NOEXCEPT_SPEC
    {
        base_t::splice(pos, other, it);
    }

    void splice(const_iterator pos, list&& other, const_iterator it) NESTL_NOEXCEPT_SPEC
    {
        base_t::splice(pos, other, it);
    }

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC
    {
        base_t::splice(pos, other, first, last);
    }

    void splice(const_iterator pos, list&& other, const_iterator first, const_iterator last) NESTL_NOEXCEPT_SPEC
    {
        base_t::splice(pos, other, first, last);
    }

private:
};

} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_LIST_HPP */
