#ifndef NESTL_HAS_EXCEPTIONS_VECTOR_HPP
#define NESTL_HAS_EXCEPTIONS_VECTOR_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/vector.hpp>
#include <nestl/has_exceptions/default_operation_error.hpp>

namespace nestl
{
namespace has_exceptions
{


template <typename T, typename Allocator = nestl::allocator<T> >
class vector: private impl::vector<T, Allocator>
{
    typedef impl::vector<T, Allocator> base_t;

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

    typedef base_t&                                 as_noexcept_reference;
    typedef const base_t&                           as_noexcept_const_reference;

    explicit vector(const allocator_type& alloc = allocator_type()) NESTL_NOEXCEPT_SPEC;

    explicit vector(vector&& other) NESTL_NOEXCEPT_SPEC;

    vector(const vector& other);

    vector(const base_t& other);

    vector(base_t&& other) NESTL_NOEXCEPT_SPEC;

    vector& operator=(const vector& other);

    vector& operator=(const base_t& other);

    as_noexcept_reference as_noexcept() NESTL_NOEXCEPT_SPEC;

    as_noexcept_const_reference as_noexcept() const NESTL_NOEXCEPT_SPEC;

    using base_t::get_allocator;
    using base_t::copy_nothrow;
    using base_t::assign_nothrow;
    using base_t::operator[];
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
    using base_t::reserve_nothrow;
    using base_t::capacity;
    using base_t::shrink_to_fit_nothrow;
    using base_t::clear;
    using base_t::insert_nothrow;
    using base_t::emplace_nothrow;
    using base_t::erase_nothrow;
    using base_t::push_back_nothrow;
    using base_t::emplace_back_nothrow;
    using base_t::pop_back;
    using base_t::resize_nothrow;
    using base_t::swap;

    void push_back(const value_type& value);

    void push_back(value_type&& value);
};


template <typename T, typename A>
vector<T, A>::vector(const allocator_type& alloc) NESTL_NOEXCEPT_SPEC
    : base_t(alloc)
{
}

template <typename T, typename A>
vector<T, A>::vector(vector&& other) NESTL_NOEXCEPT_SPEC
    : base_t(std::move(other))
{
}

template <typename T, typename A>
vector<T, A>::vector(const vector& other)
    : base_t(other.get_allocator())
{
    default_operation_error err;
    this->copy_nothrow(err, other);
    if (err)
    {
        throw_exception(err);
    }
}

template <typename T, typename A>
vector<T, A>::vector(const base_t& other)
    : base_t(other.get_allocator())
{
    default_operation_error err;
    this->copy_nothrow(err, other);
    if (err)
    {
        throw_exception(err);
    }
}

template <typename T, typename A>
vector<T, A>::vector(base_t&& other) NESTL_NOEXCEPT_SPEC
    : base_t(std::move(other))
{
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const vector& other)
{
    vector tmp(other);
    this->swap(tmp);

    return *this;
}

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator=(const base_t& other)
{
    vector tmp(other);
    this->swap(tmp);

    return *this;
}

template <typename T, typename A>
typename vector<T, A>::as_noexcept_reference
vector<T, A>::as_noexcept() NESTL_NOEXCEPT_SPEC
{
    return static_cast<as_noexcept_reference>(*this);
}

template <typename T, typename A>
typename vector<T, A>::as_noexcept_const_reference
vector<T, A>::as_noexcept() const NESTL_NOEXCEPT_SPEC
{
    return static_cast<as_noexcept_const_reference>(*this);
}

template <typename T, typename A>
void
vector<T, A>::push_back(const value_type& value)
{
    default_operation_error error;
    push_back_nothrow(error, value);
    if (error)
    {
        throw_exception(error);
    }
}

template <typename T, typename A>
void
vector<T, A>::push_back(value_type&& value)
{
    default_operation_error error;
    push_back_nothrow(error, std::move(value));
    if (error)
    {
        throw_exception(error);
    }
}

} // namespace has_exceptions
} // namespace nestl

#endif /* NESTL_HAS_EXCEPTIONS_VECTOR_HPP */
