#ifndef NESTL_ALGORITHM_HPP
#define NESTL_ALGORITHM_HPP

/**
 * @file Collection of standard algorithms
 */

#include <nestl/config.hpp>
#include <nestl/class_traits.hpp>
#include <nestl/operation_error.hpp>


namespace nestl
{

template<typename OperationError, typename InputIterator, typename OutputIterator, typename UnaryPredicate>
nestl::result_with_operation_error<OutputIterator, OperationError>
copy_if(InputIterator first, InputIterator last, OutputIterator d_first, UnaryPredicate pred)
{
    while (first != last)
    {
        if (pred(*first))
        {
            OperationError err = nestl::detail::assign<OperationError>(*d_first, *first);
            if (err)
            {
                return nestl::make_result_with_operation_error(d_first, err);
            }
            ++d_first;
        }
        ++first;
    }
    return nestl::make_result_with_operation_error(d_first, OperationError());
}

template<typename OperationError, typename InputIterator, typename OutputIterator>
nestl::result_with_operation_error<OutputIterator, OperationError>
copy(InputIterator first, InputIterator last, OutputIterator d_first)
{
    while (first != last)
    {
        OperationError err = nestl::detail::assign<OperationError>(*d_first, *first);
        if (err)
        {
            return nestl::make_result_with_operation_error(d_first, err);
        }

        ++d_first;
        ++first;
    }
    return nestl::make_result_with_operation_error(d_first, OperationError());
}

} // namespace nestl

#endif /* NESTL_ALGORITHM_HPP */
