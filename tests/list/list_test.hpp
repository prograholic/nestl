#ifndef NESTL_TESTS_LIST_LIST_TEST_HPP
#define NESTL_TESTS_LIST_LIST_TEST_HPP

#include <nestl/list.hpp>

#include "tests/test_common.hpp"

namespace nestl
{

namespace test
{

template <typename List>
::testing::AssertionResult CheckListSize(const List& l, size_t expectedSize)
{
    if (l.size() != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "expected size: " << expectedSize << ", got: " << l.size();
    }

    bool shouldBeEmpty = (expectedSize == 0);
    bool listIsEmpty = l.empty();

    if (listIsEmpty != shouldBeEmpty)
    {
        return ::testing::AssertionFailure() <<
            "list empty method returns " << listIsEmpty << ", "
            "while it should be " << shouldBeEmpty;
    }

    size_t dist = nestl::distance(l.begin(), l.end());
    if (dist != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "distance between iterators should be " << expectedSize << " elements, "
            "got " << dist << " elements";
    }

    size_t cdist = nestl::distance(l.cbegin(), l.cend());
    if (cdist != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "distance between const iterators should be " << expectedSize << " elements, "
            "got " << cdist << " elements";
    }


    return ::testing::AssertionSuccess();
}


/**
 * @brief Base class for all list tests
 */
template <typename TypeWithAllocator>
class ListTestBase : public ::testing::Test
{
public:

    typedef typename TypeWithAllocator::value_type         value_type;
    typedef typename TypeWithAllocator::allocator          allocator;

    typedef nestl::list<value_type, allocator>             list_t;
    typedef typename list_t::operation_error               operation_error;
    typedef typename list_t::const_iterator                const_iterator;
    typedef typename list_t::iterator                      iterator;
    typedef typename list_t::iterator_with_operation_error iterator_with_operation_error;
};



/**
 * @brief Type list definitions for tests
 */

typedef ::testing::Types
<
    type_with_allocator<int>,
    type_with_allocator<nestl::list<int> >,
    type_with_allocator<non_copyable>,
    type_with_allocator<nestl::list<non_copyable> >
> ListCommonTypes;

typedef ::testing::Types
<
    type_with_allocator<int>
> ListNumericTypes;

typedef ListCommonTypes ListCopyableTypes;


} // namespace test

} // namespace nestl


#endif /* NESTL_TESTS_LIST_LIST_TEST_HPP */
