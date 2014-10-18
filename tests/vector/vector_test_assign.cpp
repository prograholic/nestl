#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{


class VectorTestAssignBase : public VectorTestBase2
{
};


class VectorTestAssignWithDefaultAllocator : public VectorTestAssignBase
{
};


#if NESTL_HAS_RVALUE_REF

TEST_F(VectorTestAssignWithDefaultAllocator, MoveAssignEmptyVectorToEmptyVector)
{
    nestl::vector<int> vec1;
    nestl::vector<int> vec2;

    vec2 = nestl::move(vec1);
    EXPECT_TRUE(CheckVectorSize(vec2, 0));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}


TEST_F(VectorTestAssignWithDefaultAllocator, MoveAssignEmptyVectorToNonEmptyVector)
{
    nestl::vector<int> vec1;
    nestl::vector<int> vec2;
    ASSERT_OPERATION_SUCCESS(vec2.assign(10));

    vec2 = nestl::move(vec1);
    EXPECT_TRUE(CheckVectorSize(vec2, 0));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}


TEST_F(VectorTestAssignWithDefaultAllocator, MoveAssignNonEmptyVectorToEmptyVector)
{
    nestl::vector<int> vec1;
    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::vector<int> vec2;

    vec2 = nestl::move(vec1);
    EXPECT_TRUE(CheckVectorSize(vec2, 10));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}


TEST_F(VectorTestAssignWithDefaultAllocator, MoveAssignNonEmptyVectorToNonEmptyVector)
{
    nestl::vector<int> vec1;
    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::vector<int> vec2;
    ASSERT_OPERATION_SUCCESS(vec2.assign(5));

    vec2 = nestl::move(vec1);
    EXPECT_TRUE(CheckVectorSize(vec2, 10));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}

#endif /* NESTL_HAS_RVALUE_REF */

} // namespace test

} // namespace nestl
