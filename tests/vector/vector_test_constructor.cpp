#include "tests/vector/vector_test.hpp"

namespace nestl
{
namespace test
{


class VectorTestConstructorBase : public VectorTestBase2
{
};

class VectorTestConstructorWithDefaultAllocator : public VectorTestConstructorBase
{
};


TEST_F(VectorTestConstructorWithDefaultAllocator, ConstructorWithoutParameters)
{
    nestl::vector<int> vec;

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}


TEST_F(VectorTestConstructorWithDefaultAllocator, ConstructorWithoutParametersWithAllocator)
{
    nestl::allocator<int> a;
    nestl::vector<int> vec(a);

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithDefaultAllocator, MoveConstructorFromEmptyVector)
{
    nestl::vector<int> vec1;

    nestl::vector<int> vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 0));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */


#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithDefaultAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::vector<int> vec1;

    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::vector<int> vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 10));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */




////////////////////////////////////////////////////////////////////////////////
/// Now check with zero allocator
////////////////////////////////////////////////////////////////////////////////


class VectorTestConstructorWithZeroAllocator : public VectorTestConstructorBase
{
};


TEST_F(VectorTestConstructorWithZeroAllocator, ConstructorWithoutParameters)
{
    nestl::vector<int, zero_allocator<int> > vec;

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}


TEST_F(VectorTestConstructorWithZeroAllocator, ConstructorWithoutParametersWithAllocator)
{
    zero_allocator<int> a;
    nestl::vector<int, zero_allocator<int> > vec(a);

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithZeroAllocator, MoveConstructorFromEmptyVector)
{
    nestl::vector<int, zero_allocator<int> > vec1;

    nestl::vector<int, zero_allocator<int> > vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 0));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */




////////////////////////////////////////////////////////////////////////////////
/// Now check with minimal allocator
////////////////////////////////////////////////////////////////////////////////


class VectorTestConstructorWithMinimalAllocator : public VectorTestConstructorBase
{
};


TEST_F(VectorTestConstructorWithMinimalAllocator, ConstructorWithoutParameters)
{
    nestl::vector<int, minimal_allocator<int> > vec;

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}


TEST_F(VectorTestConstructorWithMinimalAllocator, ConstructorWithoutParametersWithAllocator)
{
    minimal_allocator<int> a;
    nestl::vector<int, minimal_allocator<int> > vec(a);

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithMinimalAllocator, MoveConstructorFromEmptyVector)
{
    nestl::vector<int, minimal_allocator<int> > vec1;

    nestl::vector<int, minimal_allocator<int> > vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 0));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithMinimalAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::vector<int, minimal_allocator<int> > vec1;

    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::vector<int, minimal_allocator<int> > vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 10));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */


////////////////////////////////////////////////////////////////////////////////
/// Now check with stateful allocator
////////////////////////////////////////////////////////////////////////////////

#if defined(NESTL_TEST_ALLOCATOR_WITH_STATE)

class VectorTestConstructorWithStatefulAllocator : public VectorTestConstructorBase
{
};

TEST_F(VectorTestConstructorWithStatefulAllocator, ConstructorWithoutParameters)
{
    nestl::vector<int, allocator_with_state<int> > vec;

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}


TEST_F(VectorTestConstructorWithStatefulAllocator, ConstructorWithoutParametersWithAllocator)
{
    allocator_with_state<int> a;
    nestl::vector<int, allocator_with_state<int> > vec(a);

    EXPECT_TRUE(CheckVectorSize(vec, 0));
}


#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithStatefulAllocator, MoveConstructorFromEmptyVector)
{
    nestl::vector<int, allocator_with_state<int> > vec1;

    nestl::vector<int, allocator_with_state<int> > vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 0));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */

#if defined(NESTL_CONFIG_HAS_RVALUE_REF)
TEST_F(VectorTestConstructorWithStatefulAllocator, MoveConstructorFromNonEmptyVector)
{
    nestl::vector<int, allocator_with_state<int> > vec1;

    ASSERT_OPERATION_SUCCESS(vec1.assign(10));

    nestl::vector<int, allocator_with_state<int> > vec2(nestl::move(vec1));

    EXPECT_TRUE(CheckVectorSize(vec2, 10));
    EXPECT_TRUE(CheckVectorSize(vec1, 0));
}
#endif /* defined(NESTL_CONFIG_HAS_RVALUE_REF) */

#endif /* defined(NESTL_TEST_ALLOCATOR_WITH_STATE) */

} // namespace test

} // namespace nestl
