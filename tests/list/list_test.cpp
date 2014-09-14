#include "tests/list/list_test.hpp"


namespace nestl
{
namespace test
{





template <typename TypeWithAllocator>
class ListTestCommon : public ListTestBase<TypeWithAllocator>
{
};


TYPED_TEST_CASE_P(ListTestCommon);


TYPED_TEST_P(ListTestCommon, ConstructorWithoutParameters)
{
    typename TestFixture::list_t l;

    EXPECT_TRUE(CheckListSize(l, 0));
}

TYPED_TEST_P(ListTestCommon, CheckAssignWithSize0)
{
    typename TestFixture::list_t l;

    ASSERT_OPERATION_SUCCESS(l.assign(0));

    EXPECT_TRUE(CheckListSize(l, 0));
}

TYPED_TEST_P(ListTestCommon, CheckAssignWithSize1)
{
    const size_t expectedSize = 1;
    typename TestFixture::list_t l;
    typename TestFixture::value_type defaultParam;

    ASSERT_OPERATION_SUCCESS(l.assign(expectedSize, defaultParam));

    ASSERT_TRUE(CheckListSize(l, expectedSize));

    EXPECT_EQ(defaultParam, *(l.begin()));
    EXPECT_EQ(defaultParam, *(l.cbegin()));
    EXPECT_EQ(defaultParam, l.front());
    EXPECT_EQ(defaultParam, l.back());
}

TYPED_TEST_P(ListTestCommon, CheckAssignWithSize1024)
{
    const size_t expectedSize = 1024;
    typename TestFixture::list_t l;
    typename TestFixture::value_type defaultParam;

    ASSERT_OPERATION_SUCCESS(l.assign(expectedSize, defaultParam));

    ASSERT_TRUE(CheckListSize(l, expectedSize));

    auto constIt = l.cbegin();
    auto it = l.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(defaultParam, *constIt);
        EXPECT_EQ(defaultParam, *it);

        ++constIt;
        ++it;
    }
}

TYPED_TEST_P(ListTestCommon, CheckInsertIntoEmptyVector)
{
    typename TestFixture::list_t l;
    typename TestFixture::value_type val;

    ASSERT_OPERATION_SUCCESS(l.insert(l.begin(), val));

    ASSERT_TRUE(CheckListSize(l, 1));

    EXPECT_EQ(val, *(l.begin()));
    EXPECT_EQ(val, *(l.cbegin()));
    EXPECT_EQ(val, l.front());
    EXPECT_EQ(val, l.back());
}


REGISTER_TYPED_TEST_CASE_P(ListTestCommon,
                           ConstructorWithoutParameters,
                           CheckAssignWithSize0,
                           CheckAssignWithSize1,
                           CheckAssignWithSize1024,
                           CheckInsertIntoEmptyVector);

INSTANTIATE_TYPED_TEST_CASE_P(common_list_test, ListTestCommon, ListCommonTypes);


////////////////////////////////////////////////////////////////////////////////

template <typename TypeWithAllocator>
class ListTestNumeric : public ListTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(ListTestNumeric);


TYPED_TEST_P(ListTestNumeric, CheckAssignFromIterators)
{
    typename TestFixture::list_t l;
    typename TestFixture::value_type values [] = {0, 1, 2, 3, 4, 5};
    const size_t expectedSize = std::distance(std::begin(values), std::end(values));

    ASSERT_OPERATION_SUCCESS(l.assign(std::begin(values), std::end(values)));

    ASSERT_TRUE(CheckListSize(l, expectedSize));

    auto constIt = l.cbegin();
    auto it = l.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(values[i], *constIt);
        EXPECT_EQ(values[i], *it);

        ++constIt;
        ++it;
    }
}



TYPED_TEST_P(ListTestNumeric, CheckAssignFromInitializerList)
{
    #define INITIALIZER_LIST_VALUES {0, 1, 2, 3, 4, 5}

    typename TestFixture::list_t l;
    typename TestFixture::value_type values [] = INITIALIZER_LIST_VALUES;
    const size_t expectedSize = std::distance(std::begin(values), std::end(values));

    ASSERT_OPERATION_SUCCESS(l.assign(INITIALIZER_LIST_VALUES));

    ASSERT_TRUE(CheckListSize(l, expectedSize));

    auto constIt = l.cbegin();
    auto it = l.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(values[i], *constIt);
        EXPECT_EQ(values[i], *it);

        ++constIt;
        ++it;
    }

    #undef INITIALIZER_LIST_VALUES
}

TYPED_TEST_P(ListTestNumeric, InsertOneElementBeforeBeginToNonEmptyVector)
{
    typename TestFixture::list_t l;
    const size_t expectedSize = 5;

    ASSERT_OPERATION_SUCCESS(l.assign({1, 2, 3, 4}));

    auto st = l.insert(l.begin(), 0);
    ASSERT_OPERATION_SUCCESS(st);
    ASSERT_TRUE(CheckListSize(l, expectedSize));
    ASSERT_EQ(l.begin(), st.result());

    auto constIt = l.cbegin();
    auto it = l.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(i, *constIt);
        EXPECT_EQ(i, *it);

        ++constIt;
        ++it;
    }
}

TYPED_TEST_P(ListTestNumeric, InsertOneElementBeforeEndToNonEmptyVector)
{
    typename TestFixture::list_t l;
    const size_t expectedSize = 5;

    ASSERT_OPERATION_SUCCESS(l.assign({0, 1, 2, 3}));

    auto st = l.insert(l.end(), 4);
    ASSERT_OPERATION_SUCCESS(st);
    ASSERT_TRUE(CheckListSize(l, expectedSize));

    ASSERT_EQ(l.end()--, st.result());

    auto constIt = l.cbegin();
    auto it = l.begin();
    for (size_t i = 0; i != expectedSize; ++i)
    {
        EXPECT_EQ(i, *constIt);
        EXPECT_EQ(i, *it);

        ++constIt;
        ++it;
    }
}



REGISTER_TYPED_TEST_CASE_P(ListTestNumeric,
                           CheckAssignFromIterators,
                           CheckAssignFromInitializerList,
                           InsertOneElementBeforeBeginToNonEmptyVector,
                           InsertOneElementBeforeEndToNonEmptyVector);

INSTANTIATE_TYPED_TEST_CASE_P(numeric_list_test, ListTestNumeric, ListNumericTypes);



////////////////////////////////////////////////////////////////////////////////

template <typename TypeWithAllocator>
class ListTestWithCopyableObjects : public ListTestBase<TypeWithAllocator>
{
};

TYPED_TEST_CASE_P(ListTestWithCopyableObjects);

TYPED_TEST_P(ListTestWithCopyableObjects, PushBackOneElement)
{
    typename TestFixture::list_t l;
    typename TestFixture::value_type val;

    ASSERT_OPERATION_SUCCESS(l.push_back(val));

    EXPECT_TRUE(CheckListSize(l, 1));
}


TYPED_TEST_P(ListTestWithCopyableObjects, EmplaceBackOneElement)
{
    typename TestFixture::list_t l;
    typename TestFixture::value_type val;

    ASSERT_OPERATION_SUCCESS(l.emplace_back(val));

    EXPECT_TRUE(CheckListSize(l, 1));
}


REGISTER_TYPED_TEST_CASE_P(ListTestWithCopyableObjects,
                           PushBackOneElement,
                           EmplaceBackOneElement);

INSTANTIATE_TYPED_TEST_CASE_P(copyable_list_test, ListTestWithCopyableObjects, ListCopyableTypes);





} // namespace test

} // namespace nestl
