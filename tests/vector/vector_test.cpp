#include "tests/vector/vector_test.hpp"


namespace nestl
{
namespace test
{





template <typename TypeWithAllocator>
class VectorTestCommon : public VectorTestBase<TypeWithAllocator>
{
};


TYPED_TEST_CASE_P(VectorTestCommon);


TYPED_TEST_P(VectorTestCommon, ConstructorWithoutParameters)
{
    typename TestFixture::vector_t vec;

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}

TYPED_TEST_P(VectorTestCommon, CheckAssignWithSize0)
{
    typename TestFixture::vector_t vec;

    ASSERT_OPERATION_SUCCESS(vec.assign(0));

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}

TYPED_TEST_P(VectorTestCommon, CheckAssignWithSize1)
{
    const size_t expectedSize = 1;
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type defaultParam;

    ASSERT_OPERATION_SUCCESS(vec.assign(expectedSize, defaultParam));

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

    ASSERT_OPERATION_SUCCESS(vec.assign(expectedSize, defaultParam));

    ASSERT_TRUE(CheckVectorSize(vec, expectedSize));

    auto constIt = vec.cbegin();
    auto it = vec.begin();
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

    ASSERT_OPERATION_SUCCESS(vec.insert(vec.begin(), val));

    ASSERT_TRUE(CheckVectorSize(vec, 1));

    EXPECT_EQ(val, *(vec.begin()));
    EXPECT_EQ(val, *(vec.cbegin()));
    EXPECT_EQ(val, vec[0]);
    EXPECT_EQ(val, vec.front());
    EXPECT_EQ(val, vec.back());
}


REGISTER_TYPED_TEST_CASE_P(VectorTestCommon,
                           ConstructorWithoutParameters,
                           CheckAssignWithSize0,
                           CheckAssignWithSize1,
                           CheckAssignWithSize1024,
                           CheckInsertIntoEmptyVector);

INSTANTIATE_TYPED_TEST_CASE_P(common_vector_test, VectorTestCommon, VectorCommonTypes);


////////////////////////////////////////////////////////////////////////////////

template <typename TypeWithAllocator>
class VectorTestNumeric : public VectorTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(VectorTestNumeric);


TYPED_TEST_P(VectorTestNumeric, CheckAssignFromIterators)
{
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type values [] = {0, 1, 2, 3, 4, 5};
    const size_t expectedSize = std::distance(std::begin(values), std::end(values));

    ASSERT_OPERATION_SUCCESS(vec.assign(std::begin(values), std::end(values)));

    ASSERT_TRUE(CheckVectorSize(vec, expectedSize));

    auto constIt = vec.cbegin();
    auto it = vec.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(values[i], *constIt);
        EXPECT_EQ(values[i], *it);
        EXPECT_EQ(values[i], vec[i]);

        ++constIt;
        ++it;
    }
}



TYPED_TEST_P(VectorTestNumeric, CheckAssignFromInitializerList)
{
    #define INITIALIZER_LIST_VALUES {0, 1, 2, 3, 4, 5}

    typename TestFixture::vector_t vec;
    typename TestFixture::value_type values [] = INITIALIZER_LIST_VALUES;
    const size_t expectedSize = std::distance(std::begin(values), std::end(values));

    ASSERT_OPERATION_SUCCESS(vec.assign(INITIALIZER_LIST_VALUES));

    ASSERT_TRUE(CheckVectorSize(vec, expectedSize));

    auto constIt = vec.cbegin();
    auto it = vec.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(values[i], *constIt);
        EXPECT_EQ(values[i], *it);
        EXPECT_EQ(values[i], vec[i]);

        ++constIt;
        ++it;
    }

    #undef INITIALIZER_LIST_VALUES
}

TYPED_TEST_P(VectorTestNumeric, InsertOneElementBeforeBeginToNonEmptyVector)
{
    typename TestFixture::vector_t vec;

    ASSERT_OPERATION_SUCCESS(vec.assign({1, 2, 3, 4}));

    auto st = vec.insert(vec.begin(), 0);
    ASSERT_OPERATION_SUCCESS(st);
    ASSERT_TRUE(CheckVectorSize(vec, 5));
    ASSERT_EQ(vec.begin(), st.result());

    EXPECT_EQ(0, vec[0]);
    EXPECT_EQ(1, vec[1]);
    EXPECT_EQ(2, vec[2]);
    EXPECT_EQ(3, vec[3]);
    EXPECT_EQ(4, vec[4]);
}

TYPED_TEST_P(VectorTestNumeric, InsertOneElementBeforeEndToNonEmptyVector)
{
    typename TestFixture::vector_t vec;

    ASSERT_OPERATION_SUCCESS(vec.assign({0, 1, 2, 3}));

    auto st = vec.insert(vec.end(), 4);
    ASSERT_OPERATION_SUCCESS(st);
    ASSERT_TRUE(CheckVectorSize(vec, 5));
    ASSERT_EQ(vec.end() - 1, st.result());

    EXPECT_EQ(0, vec[0]);
    EXPECT_EQ(1, vec[1]);
    EXPECT_EQ(2, vec[2]);
    EXPECT_EQ(3, vec[3]);
    EXPECT_EQ(4, vec[4]);
}



REGISTER_TYPED_TEST_CASE_P(VectorTestNumeric,
                           CheckAssignFromIterators,
                           CheckAssignFromInitializerList,
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

    ASSERT_OPERATION_SUCCESS(vec.push_back(val));

    EXPECT_TRUE(CheckVectorSize(vec, 1));
}


TYPED_TEST_P(VectorTestWithCopyableObjects, EmplaceBackOneElement)
{
    typename TestFixture::vector_t vec;
    typename TestFixture::value_type val;

    ASSERT_OPERATION_SUCCESS(vec.emplace_back(val));

    EXPECT_TRUE(CheckVectorSize(vec, 1));
}


REGISTER_TYPED_TEST_CASE_P(VectorTestWithCopyableObjects,
                           PushBackOneElement,
                           EmplaceBackOneElement);

INSTANTIATE_TYPED_TEST_CASE_P(copyable_vector_test, VectorTestWithCopyableObjects, VectorCopyableTypes);





} // namespace test

} // namespace nestl
