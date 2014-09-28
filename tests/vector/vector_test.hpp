#ifndef NESTL_TESTS_VECTOR_VECTOR_TEST_HPP
#define NESTL_TESTS_VECTOR_VECTOR_TEST_HPP

#include <nestl/vector.hpp>

#include "tests/test_common.hpp"

namespace nestl
{

namespace test
{

template <typename Vector>
::testing::AssertionResult CheckVectorSize(const Vector& v, size_t expectedSize)
{
    if (v.size() != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "expected size: " << expectedSize << ", got: " << v.size();
    }

    bool shouldBeEmpty = (expectedSize == 0);
    bool vectorIsEmpty = v.empty();

    if (vectorIsEmpty != shouldBeEmpty)
    {
        return ::testing::AssertionFailure() <<
            "vector empty method returns " << vectorIsEmpty << ", "
            "while it should be " << shouldBeEmpty;
    }

    if (v.capacity() < v.size())
    {
        return ::testing::AssertionFailure() <<
            "capacity should be at least " << v.size() << " elements, "
            "got " << v.capacity() << " elements";
    }


    size_t dist = std::distance(v.begin(), v.end());
    if (dist != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "distance between iterators should be " << expectedSize << " elements, "
            "got " << dist << " elements";
    }

    size_t cdist = std::distance(v.cbegin(), v.cend());
    if (cdist != expectedSize)
    {
        return ::testing::AssertionFailure() <<
            "distance between const iterators should be " << expectedSize << " elements, "
            "got " << cdist << " elements";
    }


    return ::testing::AssertionSuccess();
}


/**
 * @brief Base class for all vector tests
 */
template <typename TypeWithAllocator>
class VectorTestBase : public ::testing::Test
{
public:

    typedef typename TypeWithAllocator::value_type           value_type;
    typedef typename TypeWithAllocator::allocator            allocator;

    typedef nestl::vector<value_type, allocator>             vector_t;
    typedef typename vector_t::operation_error               operation_error;
    typedef typename vector_t::iterator_with_operation_error iterator_with_operation_error;
};


/**
 * @brief base class for all vector tests
 */
class VectorTestBase2 : public ::testing::Test
{
};



/**
 * @brief Type list definitions for tests
 */

typedef ::testing::Types
<
    type_with_allocator<int>,
    type_with_allocator<nestl::vector<int> >,
    type_with_allocator<non_copyable>,
    type_with_allocator<nestl::vector<non_copyable> >
> VectorCommonTypesWithDefaultAllocator;


typedef ::testing::Types
<
    type_with_allocator<int, zero_allocator<int> >,
    type_with_allocator<nestl::vector<int>, zero_allocator<nestl::vector<int> > >,
    type_with_allocator<non_copyable, zero_allocator<non_copyable> >,
    type_with_allocator<nestl::vector<non_copyable>, zero_allocator<nestl::vector<non_copyable> > >
> VectorCommonTypesWithZeroAllocator;




typedef ::testing::Types
<
    type_with_allocator<int>,
    type_with_allocator<nestl::vector<int> >,
    type_with_allocator<non_copyable>,
    type_with_allocator<nestl::vector<non_copyable> >,

    type_with_allocator<int, zero_allocator<int> >,
    type_with_allocator<nestl::vector<int>, zero_allocator<nestl::vector<int> > >,
    type_with_allocator<non_copyable, zero_allocator<non_copyable> >,
    type_with_allocator<nestl::vector<non_copyable>, zero_allocator<nestl::vector<non_copyable> > >
> VectorCommonTypesWithDifferentAllocators;



typedef ::testing::Types
<
    type_with_allocator<int>
> VectorNumericTypes;

typedef VectorCommonTypesWithDefaultAllocator VectorCopyableTypes;


} // namespace test

} // namespace nestl


#endif /* NESTL_TESTS_VECTOR_VECTOR_TEST_HPP */
