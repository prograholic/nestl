#ifndef NESTL_ALGORITHM_HPP
#define NESTL_ALGORITHM_HPP

/**
 * @file Collection of standard algorithms
 */

#include <nestl/config.hpp>
#include <nestl/class_traits.hpp>

#include <algorithm>

namespace nestl
{


template <typename Iterator, typename OperationError>
class iterator_operation_error
{
public:

    iterator_operation_error(Iterator it, const OperationError& err) noexcept
        : m_iter(it)
        , m_error(err)
    {
    }

    int value() const noexcept
    {
        return m_error.value();
    }

    std::string message() const
    {
        return m_error.message();
    }

    explicit operator bool() const noexcept
    {
        return value() != 0 ? true : false;
    }


private:
    Iterator m_iter;
    OperationError m_error;
};


template <typename Iterator, typename OperationError>
iterator_operation_error<Iterator, OperationError>
make_iterator_operation_error(Iterator it, const OperationError& err) noexcept
{
    return iterator_operation_error<Iterator, OperationError>(it, err);
}



template<typename OperationError, typename InputIterator, typename OutputIterator, typename UnaryPredicate>
nestl::iterator_operation_error<OutputIterator, OperationError>
copy_if(InputIterator first, InputIterator last, OutputIterator d_first, UnaryPredicate pred)
{
    while (first != last)
    {
        if (pred(*first))
        {
            OperationError err = nestl::detail::assign<OperationError>(*d_first, *first);
            if (err)
            {
                return make_iterator_operation_error(d_first, err);
            }
            ++d_first;
        }
        ++first;
    }
    return make_iterator_operation_error(d_first, OperationError());
}

template<typename OperationError, typename InputIterator, typename OutputIterator>
nestl::iterator_operation_error<OutputIterator, OperationError>
copy(InputIterator first, InputIterator last, OutputIterator d_first)
{
    while (first != last)
    {
        OperationError err = nestl::detail::assign<OperationError>(*d_first, *first);
        if (err)
        {
            return make_iterator_operation_error(d_first, err);
        }

        ++d_first;
        ++first;
    }
    return make_iterator_operation_error(d_first, OperationError());
}

} // namespace nestl

#endif /* NESTL_ALGORITHM_HPP */
