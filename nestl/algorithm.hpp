#ifndef NESTL_ALGORITHM_HPP
#define NESTL_ALGORITHM_HPP

/**
 * @file Collection of standard algorithms
 */

#include <nestl/config.hpp>
#include <nestl/class_operations.hpp>

#include <iterator>

namespace nestl
{

template<typename OperationError, typename InputIterator, typename OutputIterator, typename UnaryPredicate>
OutputIterator
copy_if(OperationError& err, InputIterator first, InputIterator last, OutputIterator d_first, UnaryPredicate pred)
{
    while (first != last)
    {
        if (pred(*first))
        {
            nestl::class_operations::assign(err, *d_first, *first);
            if (err)
            {
                return d_first;
            }
            ++d_first;
        }
        ++first;
    }
    return d_first;
}

template<typename OperationError, typename InputIterator, typename OutputIterator>
OutputIterator
copy(OperationError& err, InputIterator first, InputIterator last, OutputIterator d_first)
{
    while (first != last)
    {
        nestl::class_operations::assign(err, *d_first, *first);
        if (err)
        {
            return d_first;
        }

        ++d_first;
        ++first;
    }
    return d_first;
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate p)
{
    for ( ; first1 != last1; ++first1, ++first2)
    {
        if (!p(*first1, *first2))
        {
            return false;
        }
    }
    return true;
}



template<typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
    for ( ; first1 != last1; ++first1, ++first2)
    {
        if (!(*first1 == *first2))
        {
            return false;
        }
    }
    return true;
}


template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1,
                             InputIterator1 last1,
                             InputIterator2 first2,
                             InputIterator2 last2)
{
    for ( ; (first1 != last1) && (first2 != last2); first1++, first2++)
    {
        if (*first1 < *first2)
        {
            return true;
        }
        if (*first2 < *first1)
        {
            return false;
        }
    }
    return (first1 == last1) && (first2 != last2);
}

template<typename InputIterator1, typename InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1,
                             InputIterator1 last1,
                             InputIterator2 first2,
                             InputIterator2 last2,
                             Compare comp)
{
    for ( ; (first1 != last1) && (first2 != last2); first1++, first2++)
    {
        if (comp(*first1, *first2))
        {
            return true;
        }
        if (comp(*first2, *first1))
        {
            return false;
        }
    }
    return (first1 == last1) && (first2 != last2);
}

} // namespace nestl

#endif /* NESTL_ALGORITHM_HPP */
