#include "tests/vector/vector_test.hpp"

#include <nestl/algorithm.hpp>

namespace nestl
{
namespace test
{





template <typename TypeWithAllocator>
class VectorTestCommon : public VectorTestBase<TypeWithAllocator>
{
};


TYPED_TEST_CASE_P(VectorTestCommon);


TYPED_TEST_P(VectorTestCommon, CheckAssignWithSize0)
{
    typename TestFixture::vector_t vec;

    typename TestFixture::operation_error error;
    vec.assign_nothrow(error, 0);
    ASSERT_OPERATION_SUCCESS(error);

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}

TYPED_TEST_P(VectorTestCommon, CheckAssignWithSize1)
{
    const size_t expectedSize = 1;
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type defaultParam;

    typename TestFixture::operation_error error;
    vec.assign_nothrow(error, expectedSize, defaultParam);
    ASSERT_OPERATION_SUCCESS(error);

    ASSERT_TRUE(CheckVectorSize(vec, expectedSize));

    EXPECT_EQ(defaultParam, *(vec.begin()));
    EXPECT_EQ(defaultParam, *(vec.cbegin()));
    EXPECT_EQ(defaultParam, vec[0]);
    EXPECT_EQ(defaultParam, vec.front());
    EXPECT_EQ(defaultParam, vec.back());
}

TYPED_TEST_P(VectorTestCommon, CheckAssignWithSize1024)
{
    const size_t expectedSize = 1024;
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type defaultParam;

    typename TestFixture::operation_error error;
    vec.assign_nothrow(error, expectedSize, defaultParam);
    ASSERT_OPERATION_SUCCESS(error);

    ASSERT_TRUE(CheckVectorSize(vec, expectedSize));

    typename TestFixture::vector_t::const_iterator constIt = vec.cbegin();
    typename TestFixture::vector_t::iterator it = vec.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(defaultParam, *constIt);
        EXPECT_EQ(defaultParam, *it);
        EXPECT_EQ(defaultParam, vec[i]);

        ++constIt;
        ++it;
    }
}

TYPED_TEST_P(VectorTestCommon, CheckInsertIntoEmptyVector)
{
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type val;

    typename TestFixture::operation_error error;
    auto pos = vec.insert_nothrow(error, vec.begin(), val);
    ASSERT_OPERATION_SUCCESS(error);

    ASSERT_TRUE(CheckVectorSize(vec, 1));

    EXPECT_EQ(pos, vec.begin());
    EXPECT_EQ(val, *(vec.begin()));
    EXPECT_EQ(val, *(vec.cbegin()));
    EXPECT_EQ(val, vec[0]);
    EXPECT_EQ(val, vec.front());
    EXPECT_EQ(val, vec.back());
}


REGISTER_TYPED_TEST_CASE_P(VectorTestCommon,
                           CheckAssignWithSize0,
                           CheckAssignWithSize1,
                           CheckAssignWithSize1024,
                           CheckInsertIntoEmptyVector);

INSTANTIATE_TYPED_TEST_CASE_P(common_vector_test,
                              VectorTestCommon,
                              VectorCommonTypesWithDefaultAllocator);


////////////////////////////////////////////////////////////////////////////////

template <typename TypeWithAllocator>
class VectorTestNumeric : public VectorTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(VectorTestNumeric);


TYPED_TEST_P(VectorTestNumeric, CheckAssignFromIterators)
{
    typedef typename TestFixture::vector_t::const_iterator const_iterator;
    typedef typename TestFixture::vector_t::iterator       iterator;

    typename TestFixture::vector_t vec;
    typename TestFixture::value_type values [] = {0, 1, 2, 3, 4, 5};
    const size_t expectedSize = nestl::distance(nestl::begin(values), nestl::end(values));

    typename TestFixture::operation_error error;
    vec.assign_nothrow(error, nestl::begin(values), nestl::end(values));
    ASSERT_OPERATION_SUCCESS(error);

    ASSERT_TRUE(CheckVectorSize(vec, expectedSize));

    const_iterator constIt = vec.cbegin();
    iterator it = vec.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(values[i], *constIt);
        EXPECT_EQ(values[i], *it);
        EXPECT_EQ(values[i], vec[i]);

        ++constIt;
        ++it;
    }
}


TYPED_TEST_P(VectorTestNumeric, InsertOneElementBeforeBeginToNonEmptyVector)
{
    typename TestFixture::vector_t vec;

    typename TestFixture::operation_error error;
    
    vec.push_back_nothrow(error, 1);
    ASSERT_OPERATION_SUCCESS(error);
    
    vec.push_back_nothrow(error, 2);
    ASSERT_OPERATION_SUCCESS(error);
    
    vec.push_back_nothrow(error, 3);
    ASSERT_OPERATION_SUCCESS(error);
    
    vec.push_back_nothrow(error, 4);
    ASSERT_OPERATION_SUCCESS(error);

    typename TestFixture::vector_t::iterator pos = vec.insert_nothrow(error, vec.begin(), 0);
    ASSERT_OPERATION_SUCCESS(error);
    ASSERT_TRUE(CheckVectorSize(vec, 5));
    ASSERT_EQ(vec.begin(), pos);

    EXPECT_EQ(0, vec[0]);
    EXPECT_EQ(1, vec[1]);
    EXPECT_EQ(2, vec[2]);
    EXPECT_EQ(3, vec[3]);
    EXPECT_EQ(4, vec[4]);
}

TYPED_TEST_P(VectorTestNumeric, InsertOneElementBeforeEndToNonEmptyVector)
{
    typename TestFixture::vector_t vec;

    typename TestFixture::operation_error error;
    
    vec.push_back_nothrow(error, 0);
    ASSERT_OPERATION_SUCCESS(error);
    
    vec.push_back_nothrow(error, 1);
    ASSERT_OPERATION_SUCCESS(error);
    
    vec.push_back_nothrow(error, 2);
    ASSERT_OPERATION_SUCCESS(error);
    
    vec.push_back_nothrow(error, 3);
    ASSERT_OPERATION_SUCCESS(error);

    auto pos = vec.insert_nothrow(error, vec.end(), 4);
    ASSERT_OPERATION_SUCCESS(error);
    ASSERT_TRUE(CheckVectorSize(vec, 5));
    ASSERT_EQ(vec.end() - 1, pos);

    EXPECT_EQ(0, vec[0]);
    EXPECT_EQ(1, vec[1]);
    EXPECT_EQ(2, vec[2]);
    EXPECT_EQ(3, vec[3]);
    EXPECT_EQ(4, vec[4]);
}

REGISTER_TYPED_TEST_CASE_P(VectorTestNumeric,
                           CheckAssignFromIterators,
                           InsertOneElementBeforeBeginToNonEmptyVector,
                           InsertOneElementBeforeEndToNonEmptyVector);

INSTANTIATE_TYPED_TEST_CASE_P(numeric_vector_test, VectorTestNumeric, VectorNumericTypes);



////////////////////////////////////////////////////////////////////////////////

template <typename TypeWithAllocator>
class VectorTestWithCopyableObjects : public VectorTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(VectorTestWithCopyableObjects);

TYPED_TEST_P(VectorTestWithCopyableObjects, PushBackOneElement)
{
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type val;

    typename TestFixture::operation_error error;
    vec.push_back_nothrow(error, val);
    ASSERT_OPERATION_SUCCESS(error);

    EXPECT_TRUE(CheckVectorSize(vec, 1));
}


TYPED_TEST_P(VectorTestWithCopyableObjects, EmplaceBackOneElement)
{
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type val;

    typename TestFixture::operation_error error;
    vec.emplace_back_nothrow(error, val);
    ASSERT_OPERATION_SUCCESS(error);

    EXPECT_TRUE(CheckVectorSize(vec, 1));
}


REGISTER_TYPED_TEST_CASE_P(VectorTestWithCopyableObjects,
                           PushBackOneElement,
                           EmplaceBackOneElement);

INSTANTIATE_TYPED_TEST_CASE_P(copyable_vector_test, VectorTestWithCopyableObjects, VectorCopyableTypes);





} // namespace test

} // namespace nestl

